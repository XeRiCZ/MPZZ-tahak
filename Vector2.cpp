#include "Vector2.h"
#include <QDebug>
/*
Vector2::Vector2(float _x,float _y) : x(_x),y(_y)
{

}*/
void Vector2::rotate(float angle)
{

  float radians = angle * (3.14159265358979323846 / 180.0);

  double ca = cos(radians);
  double sa = sin(radians);

  float xn = ca * x - sa * y;
  float yn = sa *x + ca * y;
  x = xn;
  y = yn;
}


void Vector2::normalize(){
    // Normalizace dvoudimenzionalniho vektoru
    // Potřebné pro určení směru k dalšímu vektoru
  //  qDebug() << "normalizating : (" << input.x << ";" << input.y << ")";
     float magnitude =abs(sqrt((x * x) + (y * y)));


     x = (x*1.0) / magnitude;
     y = (y*1.0) / magnitude;
}

Vector2 Vector2::operator+(Vector2 input)
{
    // Součet vektorů
    Vector2 output = Vector2(x,y);
    output.x += input.x;
    output.y += input.y;
    return output;
}

Vector2 Vector2::operator-(Vector2 input)
{
    // Rozdíl vektorů
    Vector2 output = Vector2(x,y);
    output.x -= input.x;
    output.y -= input.y;
    return output;
}

Vector2 Vector2::operator*(float input)
{
    // Násobení vektoru skalarem
    Vector2 output = Vector2(x,y);
    output.x *= input * 1.0;
    output.y *= input * 1.0;;
    return output;
}

Vector2 Vector2::operator/(float input)
{
    // Dělení vektoru skalarem
    Vector2 output = Vector2(x,y);
    output.x /= input * 1.0;;
    output.y /= input * 1.0;;
    return output;
}

