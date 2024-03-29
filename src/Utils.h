/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains several functions that can be useful when programming your game.
*/
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <vector>

#include "Maths.h"

//General functions **************
long getTime();

//generic purposes fuctions
void drawGrid(float dist);

//check opengl errors
bool checkGLErrors();

std::string getPath();

Vector2 getDesktopSize( int display_index = 0 );

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

std::string AddCurrentPath( const std::string& p_sInputFile );

#endif
