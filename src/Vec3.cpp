#include "Vec3.h"

///////////////Vec2//////////////

Vec2::Vec2(float x, float y) {
    this->x = x;
    this->y = y;
}

float	Vec2::length()
{
    return (sqrtf(x * x + y * y));
}

void	Vec2::normalize()
{
    float fact = 1.0f / length();
    x *= fact;
    y *= fact;
}

void Vec2::print()
{
    printf("%f %f\n", x, y);
}

Vec2    Vec2::operator+(const Vec2 &v2){
    return (Vec2(x + v2.x, y + v2.y));
}

Vec2    Vec2::operator-(const Vec2 &v2){
    return (Vec2(x - v2.x, y - v2.y));
}

Vec2    Vec2::operator*(const Vec2 &v2){
    return (Vec2(x * v2.x, y * v2.y));
}

Vec2    Vec2::operator/(const Vec2 &v2){
    return (Vec2(x / v2.x, y / v2.y));
}

bool    Vec2::operator==(const Vec2& other) const{
	return (x == other.x && y == other.y);
}

Vec2   Vec2::operator+(const float &f){
    return (Vec2(x + f, y + f));
}

Vec2   Vec2::operator-(const float &f){
    return (Vec2(x - f, y - f));
}

Vec2   Vec2::operator*(const float &f){
    return (Vec2(x * f, y * f));
}

Vec2   Vec2::operator/(const float &f) {
    return (Vec2(x / f, y / f));
}

///////////////Vec3//////////////

Vec3::Vec3(float x1, float y1, float z1) {
    x = x1;
    y = y1;
    z = z1;
}

Vec3	Vec3::cross(Vec3 v2)
{
    Vec3 result;

    result.x = y * v2.z - z * v2.y;
    result.y = z * v2.x - x * v2.z;
    result.z = x * v2.y - y * v2.x;
    return (result);
}

Vec3	Vec3::scale(float fact)
{
    Vec3 res;
    res.x = x * fact;
    res.y = y * fact;
    res.z = z * fact;
    return (res);
}

float    Vec3::dot(Vec3 v2){
    return (x * v2.x + y * v2.y + z * v2.z);
}

float	dot(Vec3 v1, Vec3 v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float	distance(Vec3 pt1, Vec3 pt2)
{
    return (sqrt((pt2.x - pt1.x) *(pt2.x - pt1.x) + (pt2.y - pt1.y) *
                                                             (pt2.y - pt1.y) + (pt2.z - pt1.z) * (pt2.z - pt1.z)));
}

Vec3	neg_vec(Vec3 vec)
{
    vec.x *= -1;
    vec.y *= -1;
    vec.z *= -1;
    return (vec);
}

float	Vec3::length()
{
    return (sqrtf(x * x + y * y + z * z));
}

void	Vec3::normalize()
{
    float fact = 1.0f / length();
    x *= fact;
    y *= fact;
    z *= fact;
}

void Vec3::print()
{
    printf("%f %f %f\n", x, y, z);
}

Vec3    Vec3::operator+(const Vec3 &v2){
    return (Vec3(x + v2.x, y + v2.y, z + v2.z));
}

Vec3    Vec3::operator-(const Vec3 &v2){
    return (Vec3(x - v2.x, y - v2.y, z - v2.z));
}

Vec3    Vec3::operator*(const Vec3 &v2){
    return (Vec3(x * v2.x, y * v2.y, z * v2.z));
}

Vec3    Vec3::operator/(const Vec3 &v2){
    return (Vec3(x / v2.x, y / v2.y, z / v2.z));
}

bool    Vec3::operator==(const Vec3 &v2) const{
	return (x == v2.x && y == v2.y && z == v2.z);
}

Vec3   Vec3::operator+(const float &f){
    return (Vec3(x + f, y + f, z + f));
}

Vec3   Vec3::operator-(const float &f){
    return (Vec3(x - f, y - f, z - f));
}

Vec3   Vec3::operator*(const float &f){
    return (Vec3(x * f, y * f, z * f));
}

Vec3   Vec3::operator/(const float &f){
    return (Vec3(x / f, y / f, z / f));
}

Vec3&   Vec3::operator+=(const Vec3& v2){
    this->x += v2.x;
    this->y += v2.y;
    this->z += v2.z;
    return *this;
}

Vec3&   Vec3::operator-=(const Vec3& v2){
    this->x -= v2.x;
    this->y -= v2.y;
    this->z -= v2.z;
    return *this;
}

///////////////Vec4//////////////

Vec4::Vec4(float x1, float y1, float z1, float w1) {
    x = x1;
    y = y1;
    z = z1;
    w = w1;
}

float   Vec4::dot(Vec4 v2)
{
    return (x * v2.x + y * v2.y + z * v2.z);
}

Vec4    Vec4::cross(Vec4 v2)
{
    Vec4 result;

    result.x = y * v2.z - z * v2.y;
    result.y = z * v2.x - x * v2.z;
    result.z = x * v2.y - y * v2.x;
    return (result);
}

Vec4    Vec4::scale(float fact)
{
    Vec4 res;
    res.x = x * fact;
    res.y = y * fact;
    res.z = z * fact;
    return (res);
}

float	distance(Vec4 pt1, Vec4 pt2)
{
    return (sqrt((pt2.x - pt1.x) * (pt2.x - pt1.x) + (pt2.y - pt1.y) *
                                                     (pt2.y - pt1.y) + (pt2.z - pt1.z) * (pt2.z - pt1.z)));
}

Vec4	neg_vec(Vec4 vec)
{
    vec.x *= -1;
    vec.y *= -1;
    vec.z *= -1;
    return (vec);
}

float	Vec4::length()
{
    return (sqrtf(x * x + y * y + z * z));
}

void	Vec4::normalize()
{
    float	fact;
    float	len;

    /*
    if ((len = lenght()) == 0)
        len = 0.001;
    fact = 1.0f / len; */
    fact = 1.0f / length();
    x *= fact;
    y *= fact;
    z *= fact;
}

float   dot(Vec4 v1, Vec4 v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}


void Vec4::print()
{
    printf("%f %f %f %f\n", x, y, z, w);
}

Vec4 Vec4::operator+(const Vec4 &v2){
    return (Vec4(x + v2.x, y + v2.y, z + v2.z, w + v2.w));
}

Vec4 Vec4::operator-(const Vec4 &v2){
    return (Vec4(x - v2.x, y - v2.y, z - v2.z, w - v2.w));
}

Vec4 Vec4::operator*(const Vec4 &v2){
    return (Vec4(x * v2.x, y * v2.y, z * v2.z, w * v2.w));
}

Vec4 Vec4::operator/(const Vec4 &v2){
    return (Vec4(x / v2.x, y / v2.y, z / v2.z, w / v2.w));
}

Vec4 Vec4::operator+(float &f) {
    return (Vec4(x + f, y + f, z + f, w + f));
}

Vec4 Vec4::operator-(float &f) {
    return (Vec4(x - f, y - f, z - f, w - f));
}

Vec4 Vec4::operator*(float &f) {
    return (Vec4(x * f, y * f, z * f, w * f));
}

Vec4 Vec4::operator/(float &f) {
    return (Vec4(x / f, y / f, z / f, w / f));
}
