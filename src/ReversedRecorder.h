#include "../include/portaudio.h"
#include <sndfile.h>
#include <string>
using namespace std;

class ReversedRecorder
{
protected:
    const string filename;
    int rate;
    PaStream *stream;
public:
    ReversedRecorder(const string &_filename, int _rate);
    void preparePortAudio();
    void saveFile(float *data, int seconds);
    void stopStream();
    virtual void record() = 0;
    virtual ~ReversedRecorder();
};

class FixedRecorder : public ReversedRecorder
{
private:
    int seconds;
public:
    FixedRecorder(const string &filename, int rate, int _seconds);
    void record() override;
};

class InfiniteRecorder : public ReversedRecorder
{
public:
    InfiniteRecorder(const string &filename, int rate);
    void record() override;
};