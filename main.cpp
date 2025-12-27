#include <iostream>
#include <fstream>
#include <portaudio.h>
#include <vosk_api.h>
#include <cstdio>
using namespace std;

#define SAMPLE_RATE (16000)
#define FPB (256)
#define SEC (10)
#define CHANNELS (1)
#define SAMPLE_TYPE paInt16
typedef short Buff;


int main() {
    // Initialize Vosk
    vosk_set_log_level(-1);
    VoskModel *modell = vosk_model_new("E:/Dokumente/Code/James/models/de");
    VoskRecognizer *recorder = vosk_recognizer_new(modell, SAMPLE_RATE);

    // Initialize Portaudio
    PaError err;
    err  = Pa_Initialize();
    if (err != paNoError)
    {
        cerr << "Failed to initialize:" << Pa_GetErrorText(err) << "\n"; 
        return 1;
    }

    PaStream * live;
    err = Pa_OpenDefaultStream(&live, CHANNELS, 0, SAMPLE_TYPE, SAMPLE_RATE, FPB, nullptr, nullptr);
    if (err != paNoError)
    {
        cerr << "Failed to open Stream:" << Pa_GetErrorText(err) << "\n"; 
        return 1;
    }

    err = Pa_StartStream(live);
    if (err != paNoError)
    {
        cerr << "Failed to start Stream:" << Pa_GetErrorText(err) << "\n"; 
        return 1;
    }

    /*ofstream file;
    file.open("out.raw", ios::binary);
    if (!file)
    {
        cerr << "Failed to open/create file:" << "\n"; 
        return 1;       
    }*/
    
    Buff speicher[FPB * CHANNELS];
    //int alle_frames = SAMPLE_RATE * SEC;
    int aufgenommene_frames = 0;
    cout << "Spreche etwas.." << "\n";
    while (aufgenommene_frames >= 0)
    {
        int todo_frames = FPB;
        err = Pa_ReadStream(live, speicher, todo_frames);
        if (err && err != paInputOverflowed) {
            cerr << "Error reading stream: " << Pa_GetErrorText(err) << "\n";
            return 1;
        }
      //  file.write(reinterpret_cast<char*>(speicher), sizeof(Buff) * CHANNELS * todo_frames);
        int err_code = vosk_recognizer_accept_waveform(recorder, reinterpret_cast<char*>(speicher), sizeof(Buff) * CHANNELS * todo_frames);
        if (err_code == -1)
        {
            cerr << "Error ";
        }
        else if (err_code == 1)
        {
            const char* res = vosk_recognizer_result(recorder);
            cout << res << endl;
            if (strstr(res, "stop") != nullptr) {
                return 0;
            }
        }
        
        aufgenommene_frames += todo_frames;
    }
    const char *  text = vosk_recognizer_result(recorder);
    Pa_StopStream(live);
    Pa_CloseStream(live);
    Pa_Terminate();
    //file.close();
    vosk_model_free(modell);
    vosk_recognizer_free(recorder);
    cout << text << "\n";
    cout << "fertig! \n";

    return 0;
}
