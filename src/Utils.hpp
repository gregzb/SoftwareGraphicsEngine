#pragma once

class Color
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    
    Color();
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    Color(int r, int g, int b, int a);
};

int sign(double x);
double inverseLerp(double a, double b, double val);
int lerp(double a, double b, double t);