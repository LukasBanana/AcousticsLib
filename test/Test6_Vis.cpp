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


// ----- GLOBALS -----

Gs::Vector2i resolution(640, 480);

std::shared_ptr<Ac::AudioSystem>    audioSys;
std::unique_ptr<Ac::WaveBuffer>     waveBuffer;
std::unique_ptr<Ac::Renderer>       renderer;
std::unique_ptr<Ac::Sound>          sound;


// ----- CLASSES -----

class Renderer : public Ac::Renderer
{
public:
    
    void BeginDrawing(const Gs::Vector2ui& size) override
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
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void initAudio()
{
    // load audio system and wave buffer
    audioSys = Ac::AudioSystem::Load();
    waveBuffer = audioSys->ReadAudioBuffer("in/thorndike.wav");
    renderer = std::unique_ptr<Renderer>(new Renderer());
    sound = audioSys->CreateSound(*waveBuffer);
    if (sound)
        sound->SetVolume(0.25f);
}

void drawScene2D()
{
    // setup projection
    glMatrixMode(GL_PROJECTION);
    auto proj = Gs::ProjectionMatrix4::Planar(resolution.x, resolution.y);
    glLoadMatrixf(proj.Ptr());

    // setup model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw wave buffer
    glColor4f(0.3f, 0.5f, 1.0f, 1.0f);
    Ac::Visualizer::DrawWaveBuffer(*renderer, *waveBuffer, 0, { resolution.x, resolution.y/2 });
    
    //glColor4f(0.4f, 1.0f, 0.3f, 1.0f);
    //Ac::Visualizer::DrawWaveBuffer(*renderer, *waveBuffer, 1, { resolution.x, resolution.y/2 });
    
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(resolution.x, resolution.y);
    glutInitWindowPosition(350, 250);
    glutCreateWindow("GeometronLib Test 2 (OpenGL, GLUT)");

    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutIdleFunc(idleCallback);
    glutSpecialFunc(specialCallback);
    glutKeyboardFunc(keyboardCallback);

    initGL();
    initAudio();

    glutMainLoop();

    return 0;
}

