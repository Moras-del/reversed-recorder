//
// Created by szymon on 07.01.2021.
//

#include "ReversedRecorder.h"
#include "portaudio.h"
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


template <typename T>
void reverse_array(T *arr, int size);
void copy(float *buffer, int length, vector<float> &container);
void condition_checker(atomic<bool>& flag);
SF_INFO get_info(int rate, int seconds);

ReversedRecorder::ReversedRecorder(const string &_filename, int _rate): filename(_filename), rate(_rate){
    preparePortAudio();
}

void ReversedRecorder::preparePortAudio() {
    Pa_Initialize();
    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    Pa_OpenStream(
            &stream,
            &inputParameters,
            NULL,
            rate,
            rate,
            paClipOff,
            NULL,
            NULL);
}

void ReversedRecorder::saveFile(float *data, int seconds) {
    SF_INFO info = get_info(rate, seconds);
    SNDFILE *file = sf_open(filename.c_str(), SFM_WRITE, &info);
    sf_write_float(file, data, rate*seconds);
    sf_close(file);
    cout << filename << " saved successfully." << endl;
}

void ReversedRecorder::stopStream() {
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
}

ReversedRecorder::~ReversedRecorder() {
    Pa_Terminate();
}


FixedRecorder::FixedRecorder(const string &filename, int rate, int _seconds) : ReversedRecorder(filename, rate), seconds(_seconds) {}


void FixedRecorder::record() {
    float *buffer = new float[rate*seconds];
    Pa_StartStream(stream);
    cout << "Recording started for " << seconds << " seconds." << endl;
    for (int i = 0; i < seconds; i++) {
        Pa_ReadStream(stream, buffer, rate);
        buffer += rate;
    }
    stopStream();
    buffer -= rate*seconds;
    reverse_array(buffer, rate*seconds);
    saveFile(buffer, seconds);
    delete[] buffer;
}

InfiniteRecorder::InfiniteRecorder(const string &filename, int rate) : ReversedRecorder(filename, rate) {}

void InfiniteRecorder::record() {
    Pa_StartStream(stream);
    vector<float> data(rate);
    atomic<bool> flag = {true};
    thread t(condition_checker, ref(flag));
    cout << "Recording started. Press any key to stop." << endl;
    while (flag)
    {
        float buffer[rate];
        Pa_ReadStream(stream, buffer, rate);
        copy(buffer, rate, data);
    }
    t.join();
    stopStream();
    long data_len = data.size();
    int seconds = data.size()/rate;
    float *result = new float[data_len];
    for(int i = 0; i < data_len; i++)
        result[i] = data[data_len-i-1];
    saveFile(result, seconds);
    delete[] result;
}


template <typename T>
void reverse_array(T *arr, int size)
{
    for(int i = 0; i < size/2; i++)
    {
        T temp = arr[i];
        arr[i] = arr[size-i-1];
        arr[size-i-1] = temp;
    }
}

void condition_checker(atomic<bool>& flag)
{
    string txt;
    cin >> txt;
    flag = !flag;
}

void copy(float *buffer, int length, vector<float> &container)
{
    for(int i = 0; i < length; i++)
        container.push_back(buffer[i]);
}

SF_INFO get_info(int rate, int seconds) {
    SF_INFO info;
    info.samplerate = rate;
    info.channels = 1;
    info.frames = rate*seconds;
    info.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
    return info;
}

