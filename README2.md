# reversed-recorder
program to record microphone input, reverse it, and save it to specified file.  
Usage:  
```./reversed_recorder -h``` prints help  
```./reversed_recorder --rate=[number]``` sets rate of the record to [number] (default is 44100)  
```./reversed_recorder --time=[number]``` sets time of the record to [number]. If not specified, program will record until user press anything to the console  
Sndfile project must be installed: ```sudo apt-get install libsndfile-dev```  
Same as PortAudio: http://www.portaudio.com/docs/v19-doxydocs/tutorial_start.html
