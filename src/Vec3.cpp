#include "Vec3.h"

///////////////Vec2//////////////

Vec2::Vec2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vec2::Vec2(const Vec3 &vec3) {
    this->x = vec3.x;
    this->y = vec3.y;
}

Vec2::Vec2(const Vec4 &vec4) {
    this->x = vec4.x;
    this->y = vec4.y;
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

Vec2    Vec2::operator+(const Vec2 &rhs){
    return (Vec2(x + rhs.x, y + rhs.y));
}

Vec2    Vec2::operator-(const Vec2 &rhs){
    return (Vec2(x - rhs.x, y - rhs.y));
}

Vec2    Vec2::operator*(const Vec2 &rhs){
    return (Vec2(x * rhs.x, y * rhs.y));
}

Vec2    Vec2::operator/(const Vec2 &rhs){
    return (Vec2(x / rhs.x, y / rhs.y));
}

bool    Vec2::operator==(const Vec2& rhs) const{
	return (x == rhs.x && y == rhs.y);
}

Vec2   Vec2::operator+(const float &rhs){
    return (Vec2(x + rhs, y + rhs));
}

Vec2   Vec2::operator-(const float &rhs){
    return (Vec2(x - rhs, y - rhs));
}

Vec2   Vec2::operator*(const float &rhs){
    return (Vec2(x * rhs, y * rhs));
}

Vec2   Vec2::operator/(const float &rhs) {
    return (Vec2(x / rhs, y / rhs));
}

///////////////Vec3//////////////

Vec3::Vec3(float x1, float y1, float z1) {
    x = x1;
    y = y1;
    z = z1;
}

Vec3::Vec3(const Vec2 &vec2, float z1){
    x = vec2.x;
    y = vec2.y;
    z = z1;
}

Vec3::Vec3(const Vec4 &vec4){
    x = vec4.x;
    y = vec4.y;
    z = vec4.z;
}

Vec3	Vec3::cross(Vec3 rhs)
{
    Vec3 result;

    result.x = y * rhs.z - z * rhs.y;
    result.y = z * rhs.x - x * rhs.z;
    result.z = x * rhs.y - y * rhs.x;
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

float    Vec3::dot(Vec3 rhs){
    return (x * rhs.x + y * rhs.y + z * rhs.z);
}

float	dot(Vec3 v1, Vec3 rhs)
{
    return (v1.x * rhs.x + v1.y * rhs.y + v1.z * rhs.z);
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

Vec3	normalize(const Vec3 &rhs) {
	Vec3 vec(rhs.x, rhs.y, rhs.z);
	vec.normalize();
	return (vec);
}

void Vec3::print()
{
    printf("%f %f %f\n", x, y, z);
}

Vec3    Vec3::operator+(const Vec3 &rhs){
    return (Vec3(x + rhs.x, y + rhs.y, z + rhs.z));
}

Vec3    Vec3::operator-(const Vec3 &rhs){
    return (Vec3(x - rhs.x, y - rhs.y, z - rhs.z));
}

Vec3	Vec3::operator-() const {
	return (Vec3(-x, -y, -z));
}

Vec3    Vec3::operator*(const Vec3 &rhs){
    return (Vec3(x * rhs.x, y * rhs.y, z * rhs.z));
}

Vec3    Vec3::operator/(const Vec3 &rhs){
    return (Vec3(x / rhs.x, y / rhs.y, z / rhs.z));
}

bool    Vec3::operator==(const Vec3 &rhs) const{
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}

Vec3   Vec3::operator+(const float &rhs){
    return (Vec3(x + rhs, y + rhs, z + rhs));
}

Vec3   Vec3::operator-(const float &rhs){
    return (Vec3(x - rhs, y - rhs, z - rhs));
}

Vec3   Vec3::operator*(const float &rhs){
    return (Vec3(x * rhs, y * rhs, z * rhs));
}

Vec3   Vec3::operator/(const float &rhs){
    return (Vec3(x / rhs, y / rhs, z / rhs));
}

Vec3&   Vec3::operator+=(const Vec3& rhs){
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

Vec3&   Vec3::operator-=(const Vec3& rhs){
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

///////////////Vec4//////////////

Vec4::Vec4(float x1, float y1, float z1, float w1) {
    x = x1;
    y = y1;
    z = z1;
    w = w1;
}

Vec4::Vec4(const Vec2 &vec2, float z1, float w1) {
    x = vec2.x;
    y = vec2.y;
    z = z1;
    w = w1;
}

Vec4::Vec4(const Vec3 &vec3, float w1) {
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;
    w = w1;
}

float   Vec4::dot(Vec4 rhs)
{
    return (x * rhs.x + y * rhs.y + z * rhs.z);
}

Vec4    Vec4::cross(Vec4 rhs)
{
    Vec4 result;

    result.x = y * rhs.z - z * rhs.y;
    result.y = z * rhs.x - x * rhs.z;
    result.z = x * rhs.y - y * rhs.x;
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

Vec4 Vec4::operator+(const Vec4 &rhs){
    return (Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w));
}

Vec4 Vec4::operator-(const Vec4 &rhs){
    return (Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w));
}

Vec4 Vec4::operator*(const Vec4 &rhs){
    return (Vec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w));
}

Vec4 Vec4::operator/(const Vec4 &rhs){
    return (Vec4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w));
}

Vec4 Vec4::operator+(float &rhs) {
    return (Vec4(x + rhs, y + rhs, z + rhs, w + rhs));
}

Vec4 Vec4::operator-(float &rhs) {
    return (Vec4(x - rhs, y - rhs, z - rhs, w - rhs));
}

Vec4 Vec4::operator*(float &rhs) {
    return (Vec4(x * rhs, y * rhs, z * rhs, w * rhs));
}

Vec4 Vec4::operator/(float &rhs) {
    return (Vec4(x / rhs, y / rhs, z / rhs, w / rhs));
}


