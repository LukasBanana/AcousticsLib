/*
 * Test6_Vis.cpp
 *
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TestUtil.h"

#include <Gauss/Gauss.h>

#if defined(_WIN32)
#   include <Windows.h>
#   include <gl/glut.h>
#elif defined(__APPLE__)
#   include <GLUT/GLUT.h>
#elif defined(__linux__)
#   include <GL/glut.h>
#endif


// ----- MACROS -----

#define SHOW_WAVE_SECTION


// ----- GLOBALS -----

Gs::Vector2i resolution(640, 480);

std::shared_ptr<Ac::AudioSystem>    audioSys;
std::unique_ptr<Ac::Renderer>       renderer;
std::unique_ptr<Ac::Sound>          sound;
Ac::WaveBuffer                      waveBuffer;


// ----- CLASSES -----

class Renderer : public Ac::Renderer
{
public:
    
    void BeginDrawing(const Gs::Vector2i& size) override
    {
    }
    
    void EndDrawing() override
    {
    }
    
    void DrawLineList(const std::vector<Gs::Vector2i>& vertices) override
    {
        glBegin(GL_LINES);
        
        for (const auto& v : vertices)
            glVertex2i(v.x, v.y + resolution.y/4);
        
        glEnd();
    }
    
    void DrawLineStrip(const std::vector<Gs::Vector2i>& vertices) override
    {
        glBegin(GL_LINE_STRIP);
        
        for (const auto& v : vertices)
            glVertex2i(v.x, v.y);
        
        glEnd();
    }
    
};


// ----- FUNCTIONS -----

void initGL()
{
    // setup GL configuration
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void initAudio()
{
    // load audio system and wave buffer
    audioSys = Ac::AudioSystem::Load("XAudio2");
    
    #if 0

    // READ WAVE BUFFER

    waveBuffer = audioSys->ReadWaveBuffer(
        //"in/thorndike.wav"
        //"in/shutter.wav"
        "in/Blow.aiff"
        //"in/M1F1-AlawC-AFsp.aif"
    );

    #elif 0

    // ACCUMULATED SINE WAVES

    waveBuffer.SetTotalTime(3.0);

    waveBuffer.ForEachSample(
        [](double& sample, unsigned short channel, std::size_t index, double timePoint)
        {
            auto f = 200.0*timePoint;
            auto a1 = 0.5;
            auto a2 = 0.2;
            auto a3 = 0.08;
            auto a4 = 0.1;
            auto a5 = 0.1;
            auto a6 = 0.15;

            sample = (
                a1*std::sin(2.0*M_PI*f) +
                a2*std::sin(2.0*M_PI*f*2.0) +
                a3*std::sin(2.0*M_PI*f*3.0) +
                a4*std::sin(2.0*M_PI*f*4.0) +
                a5*std::sin(2.0*M_PI*f*5.0) +
                a6*std::sin(2.0*M_PI*f*6.0)
            );
        }
    );

    #elif 0

    // NOISE GENERATION

    waveBuffer.SetTotalTime(3.0);

    double state = 0.0;

    auto whiteNoiseGen = Ac::Synthesizer::WhiteNoiseGenerator(0.25);
    auto brownNoiseGen = Ac::Synthesizer::BrownNoiseGenerator(0.25, state);

    waveBuffer.ForEachSample(
        #if 0

        [](double& sample, unsigned short channel, std::size_t index, double timePoint)
        {
            auto amplitude = 0.5;
            auto pos = std::fmod(timePoint*100.0, 10.0);
            auto height = 1.0 / (1.0 + std::pow(std::abs(pos - 2.0), 2.0)) - 0.1;
            height = std::max(0.0, height);
            sample = std::sin(timePoint*2000.0)*amplitude*height;
        }

        #elif 1

        [&](double& sample, unsigned short channel, std::size_t index, double timePoint)
        {
            double s0 = sample, s1 = sample;
            brownNoiseGen(s0, channel, index, timePoint);
            whiteNoiseGen(s1, channel, index, timePoint);
            sample = Gs::Lerp(s0, s1, std::max(0.0, std::min((timePoint - 0.5)/2, 1.0)));
        }

        #elif 0
        whiteNoiseGen
        #else
        brownNoiseGen
        #endif
    );

    #elif 1

    // COPY AND FADING

    waveBuffer.SetTotalTime(3.0);

    double state = 0.0;

    Ac::WaveBuffer srcBuffer0(waveBuffer.GetFormat());
    Ac::WaveBuffer srcBuffer1(waveBuffer.GetFormat());
    Ac::WaveBuffer srcBuffer2(Ac::WaveBufferFormat(Ac::sampleRate32kHz, 16, 1));

    srcBuffer0.SetTotalTime(waveBuffer.GetTotalTime());
    srcBuffer1.SetTotalTime(waveBuffer.GetTotalTime());
    srcBuffer2.SetTotalTime(waveBuffer.GetTotalTime());

    srcBuffer0.ForEachSample(Ac::Synthesizer::WhiteNoiseGenerator(0.25));
    srcBuffer1.ForEachSample(Ac::Synthesizer::BrownNoiseGenerator(0.25, state));
    srcBuffer2.ForEachSample(Ac::Synthesizer::SineGenerator(0.35, 0.0, 500.0));

    Ac::Synthesizer::FadeWaveBuffers(waveBuffer, srcBuffer0, srcBuffer1, 0.5, 1.5, nullptr, false);
    Ac::Synthesizer::FadeWaveBuffers(waveBuffer, srcBuffer1, srcBuffer2, 1.5, 2.5, nullptr, false);
    waveBuffer.CopyFrom(srcBuffer0, 0.0, 0.5, 0.0);
    waveBuffer.CopyFrom(srcBuffer2, 2.5, 3.0, 2.5);

    #endif
    
    renderer = std::unique_ptr<Renderer>(new Renderer());
    
    sound = audioSys->CreateSound(waveBuffer);
    
    if (sound)
    {
        sound->SetVolume(0.25f);
        //sound->SetPitch(0.5f);
    }
}

void drawScene2D()
{
    // setup projection
    glMatrixMode(GL_PROJECTION);
    auto res = resolution.Cast<float>();
    auto proj = Gs::ProjectionMatrix4::Planar(res.x, res.y);
    glLoadMatrixf(proj.Ptr());

    // setup model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw wave buffer
    const auto seekDelta = 0.05;
    double seek = (sound != nullptr ? sound->GetSeek() : 0.0);

    glColor4f(0.3f, 0.5f, 1.0f, 1.0f);
    Ac::Visualizer::DrawWaveBuffer(
        *renderer, waveBuffer, 0,
        { 0, 0 }, { resolution.x, resolution.y/2 }
        #ifdef SHOW_WAVE_SECTION
        , seek, seek + seekDelta
        #endif
    );
    
    // draw playback line
    if (sound)
    {
        glColor4f(1, 1, 1, 1);
        glBegin(GL_LINES);
        {
            auto x = static_cast<int>((sound->GetSeek() / sound->TotalTime()) * resolution.x);
            glVertex2i(x, 0);
            glVertex2i(x, resolution.y);
        }
        glEnd();
    }
}

void displayCallback()
{
    // draw frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawScene2D();
    
    glutSwapBuffers();
}

void idleCallback()
{
    glutPostRedisplay();
}

void reshapeCallback(GLsizei w, GLsizei h)
{
    resolution.x = w;
    resolution.y = h;

    glViewport(0, 0, resolution.x, resolution.y);

    displayCallback();
}

void keyboardCallback(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // ESC
            exit(0);
            break;
            
        case ' ':
            if (sound)
                sound->Stop();
            break;

        case '\r':
            if (sound)
            {
                if (sound->IsPlaying())
                    sound->Pause();
                else
                    sound->Play();
            }
            break;
    }
}

void specialCallback(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            break;

        case GLUT_KEY_DOWN:
            break;
    }
}

int main(int argc, char* argv[])
{
    try
    {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

        auto sx = glutGet(GLUT_SCREEN_WIDTH);
        auto sy = glutGet(GLUT_SCREEN_HEIGHT);

        glutInitWindowSize(resolution.x, resolution.y);
        glutInitWindowPosition(sx/2 - resolution.x/2, sy/2 - resolution.y/2);
        glutCreateWindow("GeometronLib Test 2 (OpenGL, GLUT)");

        glutDisplayFunc(displayCallback);
        glutReshapeFunc(reshapeCallback);
        glutIdleFunc(idleCallback);
        glutSpecialFunc(specialCallback);
        glutKeyboardFunc(keyboardCallback);

        initGL();
        initAudio();

        glutMainLoop();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        #ifdef _WIN32
        system("pause");
        #endif
    }

    return 0;
}

