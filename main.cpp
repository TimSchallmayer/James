#include <iostream>
#include <fstream>
#include <portaudio.h>
#include <vosk_api.h>
#include <cstdio>
#include "external/json/json.hpp"
#include <string>
using namespace std;
using json = nlohmann::json;

#define SAMPLE_RATE (16000)
#define FPB (192)
#define SEC (10)
#define CHANNELS (1)
#define SAMPLE_TYPE paInt16
typedef short Buff;

bool is_white_space(string& s);

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
    cout << "Spreche etwas: ..." << flush;
    string current_txt = "";
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
        if (err_code == 1)
        {
            const char* res = vosk_recognizer_final_result(recorder);
            json data = json::parse(res);
            string text = data["text"];
            cout << text << endl;
            if (strstr(res, "ausschalten") != nullptr) {
                return 0;
            }
        }
            /*
        if (err_code != -1) {
            const char* res_p = vosk_recognizer_partial_result(recorder);
            json data_p = json::parse(res_p);
            string input = data_p["partial"];
            if (current_txt != input)
            {
                if(input.find(current_txt) == string::npos) {
                    current_txt = input;
                    continue;
                }
                size_t pos = input.find(current_txt);
                string input_cpy = input;
                string text = input_cpy.erase(pos, pos + current_txt.length());
                cout << text << flush;
                if (text.find("stop") != string::npos)
                {
                    return 0;
                }
                
                current_txt = input;
            }
        }*/
        else if (err_code == -1)
        {
            cerr << "Error ";
        }
        
        aufgenommene_frames += todo_frames;
    }
    Pa_StopStream(live);
    Pa_CloseStream(live);
    Pa_Terminate();
    //file.close();
    vosk_model_free(modell);
    vosk_recognizer_free(recorder);
    cout << "fertig! \n";

    return 0;
}

bool is_white_space(string& s) {
    return s.find_first_of(" \t\n\r\f\v") == string::npos;
}