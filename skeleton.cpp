#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModelH.h"
#include <stdint.h>

using namespace std;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

SDL_Event event;

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256
#define FULLSCREEN_MODE false

// Variables
vec4 cameraPos(0.0, 0.0, -3.0, 1.0); // camera position
vector<Triangle> triangles;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

bool Update();
void Draw(screen* screen);
void VertexShader( const vec4& v, glm::ivec2& p );

int main( int argc, char* argv[] )
{

  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );

  /* load model*/
  LoadTestModel(triangles);

  while ( Update())
    {
      Draw(screen);
      SDL_Renderframe(screen);
    }

  SDL_SaveImage( screen, "screenshot.bmp" );

  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

  for( uint32_t i=0; i<triangles.size(); ++i )
    {
      vector<vec4> vertices(3);

      vertices[0] = triangles[i].v0;
      vertices[1] = triangles[i].v1;
      vertices[2] = triangles[i].v2;
      for(int v=0; v<3; ++v)
        {
          glm::ivec2 projPos;
          VertexShader( vertices[v], projPos );
          vec3 color(1,1,1);
          PutPixelSDL( screen, projPos.x, projPos.y, color );
        }
  }
}

/*Place updates of parameters here*/
bool Update()
{
  static int t = SDL_GetTicks();
  /* Compute frame time */
  int t2 = SDL_GetTicks();
  float dt = float(t2-t);
  t = t2;

  SDL_Event e;
  while(SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
	{
	  return false;
	}
      else
	if (e.type == SDL_KEYDOWN)
	  {
	    int key_code = e.key.keysym.sym;
	    switch(key_code)
	      {
	      case SDLK_UP:
		/* Move camera forward */
		break;
	      case SDLK_DOWN:
		/* Move camera backwards */
		break;
	      case SDLK_LEFT:
		/* Move camera left */
		break;
	      case SDLK_RIGHT:
		/* Move camera right */
		break;
	      case SDLK_ESCAPE:
		/* Move camera quit */
		return false;
	      }
	  }
    }
  return true;
}

vec4 TransformationMatrix(glm::mat4x4 R, vec4 P, vec3 C)
{
  glm::mat4 c1 = glm::mat4{
    1, 0, 0, C.x,
    0, 1, 0, C.y,
    0, 0, 1, C.z,
    0, 0, 0, 1,
  };
  glm::mat4 c2 = glm::mat4{
    1, 0, 0, -C.x,
    0, 1, 0, -C.y,
    0, 0, 1, -C.z,
    0, 0, 0, 1,
  };
  vec4 pp = c1 * R * c2 * P;
  return pp;
}

void VertexShader( const vec4& v, glm::ivec2& p )
{
  float px = 4 * v.x / v.z + SCREEN_WIDTH/2;
  float py = 4 * v.y / v.z + SCREEN_HEIGHT/2;
  p.x = (int) px;
  p.y = (int) py;
}
