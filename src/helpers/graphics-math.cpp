#include "graphics-math.hpp"

/**
 * Takes two 4-by-4 matrices, a and b, and computes the product in the order
 * that pre-composes b with a.  In other words, the matrix returned will
 * transform by b first and then a.  Note this is subtly different from just
 * multiplying the matrices together.  For given a and b, this function returns
 * the same object in both row-major and column-major mode.
 */
auto multiply(Matrix4 a, Matrix4 b) -> Matrix4 {
  Matrix4 res;

  auto b00 = b[0 * 4 + 0];
  auto b01 = b[0 * 4 + 1];
  auto b02 = b[0 * 4 + 2];
  auto b03 = b[0 * 4 + 3];
  auto b10 = b[1 * 4 + 0];
  auto b11 = b[1 * 4 + 1];
  auto b12 = b[1 * 4 + 2];
  auto b13 = b[1 * 4 + 3];
  auto b20 = b[2 * 4 + 0];
  auto b21 = b[2 * 4 + 1];
  auto b22 = b[2 * 4 + 2];
  auto b23 = b[2 * 4 + 3];
  auto b30 = b[3 * 4 + 0];
  auto b31 = b[3 * 4 + 1];
  auto b32 = b[3 * 4 + 2];
  auto b33 = b[3 * 4 + 3];
  auto a00 = a[0 * 4 + 0];
  auto a01 = a[0 * 4 + 1];
  auto a02 = a[0 * 4 + 2];
  auto a03 = a[0 * 4 + 3];
  auto a10 = a[1 * 4 + 0];
  auto a11 = a[1 * 4 + 1];
  auto a12 = a[1 * 4 + 2];
  auto a13 = a[1 * 4 + 3];
  auto a20 = a[2 * 4 + 0];
  auto a21 = a[2 * 4 + 1];
  auto a22 = a[2 * 4 + 2];
  auto a23 = a[2 * 4 + 3];
  auto a30 = a[3 * 4 + 0];
  auto a31 = a[3 * 4 + 1];
  auto a32 = a[3 * 4 + 2];
  auto a33 = a[3 * 4 + 3];

  res[0] = b00 * a00 + b01 * a10 + b02 * a20 + b03 * a30;
  res[1] = b00 * a01 + b01 * a11 + b02 * a21 + b03 * a31;
  res[2] = b00 * a02 + b01 * a12 + b02 * a22 + b03 * a32;
  res[3] = b00 * a03 + b01 * a13 + b02 * a23 + b03 * a33;
  res[4] = b10 * a00 + b11 * a10 + b12 * a20 + b13 * a30;
  res[5] = b10 * a01 + b11 * a11 + b12 * a21 + b13 * a31;
  res[6] = b10 * a02 + b11 * a12 + b12 * a22 + b13 * a32;
  res[7] = b10 * a03 + b11 * a13 + b12 * a23 + b13 * a33;
  res[8] = b20 * a00 + b21 * a10 + b22 * a20 + b23 * a30;
  res[9] = b20 * a01 + b21 * a11 + b22 * a21 + b23 * a31;
  res[10] = b20 * a02 + b21 * a12 + b22 * a22 + b23 * a32;
  res[11] = b20 * a03 + b21 * a13 + b22 * a23 + b23 * a33;
  res[12] = b30 * a00 + b31 * a10 + b32 * a20 + b33 * a30;
  res[13] = b30 * a01 + b31 * a11 + b32 * a21 + b33 * a31;
  res[14] = b30 * a02 + b31 * a12 + b32 * a22 + b33 * a32;
  res[15] = b30 * a03 + b31 * a13 + b32 * a23 + b33 * a33;

  return res;
}

auto normalize(Vec3 v) -> Vec3 {
  Vec3 res;

  auto length = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

  // make sure we don't divide by 0.
  static const float DELTA = 0.00001;
  if (length > DELTA) {
    res[0] = v[0] / length;
    res[1] = v[1] / length;
    res[2] = v[2] / length;
  }

  return res;
}

auto subtractVectors(Vec3 a, Vec3 b) -> Vec3 {
  Vec3 res;

  res[0] = a[0] - b[0];
  res[1] = a[1] - b[1];
  res[2] = a[2] - b[2];

  return res;
}

/**
 * Computes the cross product of 2 vectors
 */
auto cross(Vec3 a, Vec3 b) -> Vec3 {
  Vec3 res;

  res[0] = a[1] * b[2] - a[2] * b[1];
  res[1] = a[2] * b[0] - a[0] * b[2];
  res[2] = a[0] * b[1] - a[1] * b[0];

  return res;
}

/**
 * Creates a lookAt matrix.
 * This is a world matrix for a camera. In other words it will transform
 * from the origin to a place and orientation in the world. For a view
 * matrix take the inverse of this.
 */
auto lookAt(Vec3 camera_pos, Vec3 target, Vec3 up) -> Matrix4 {
  Matrix4 res;

  auto zAxis = normalize(subtractVectors(camera_pos, target));
  auto xAxis = normalize(cross(up, zAxis));
  auto yAxis = normalize(cross(zAxis, xAxis));

  res[0] = xAxis[0];
  res[1] = xAxis[1];
  res[2] = xAxis[2];
  res[3] = 0;
  res[4] = yAxis[0];
  res[5] = yAxis[1];
  res[6] = yAxis[2];
  res[7] = 0;
  res[8] = zAxis[0];
  res[9] = zAxis[1];
  res[10] = zAxis[2];
  res[11] = 0;
  res[12] = camera_pos[0];
  res[13] = camera_pos[1];
  res[14] = camera_pos[2];
  res[15] = 1;

  return res;
}

/**
 * Computes the inverse of a matrix
 */
auto inverse(Matrix4 m) -> Matrix4 {
  Matrix4 res;
  auto m00 = m[0 * 4 + 0];
  auto m01 = m[0 * 4 + 1];
  auto m02 = m[0 * 4 + 2];
  auto m03 = m[0 * 4 + 3];
  auto m10 = m[1 * 4 + 0];
  auto m11 = m[1 * 4 + 1];
  auto m12 = m[1 * 4 + 2];
  auto m13 = m[1 * 4 + 3];
  auto m20 = m[2 * 4 + 0];
  auto m21 = m[2 * 4 + 1];
  auto m22 = m[2 * 4 + 2];
  auto m23 = m[2 * 4 + 3];
  auto m30 = m[3 * 4 + 0];
  auto m31 = m[3 * 4 + 1];
  auto m32 = m[3 * 4 + 2];
  auto m33 = m[3 * 4 + 3];
  auto tmp_0 = m22 * m33;
  auto tmp_1 = m32 * m23;
  auto tmp_2 = m12 * m33;
  auto tmp_3 = m32 * m13;
  auto tmp_4 = m12 * m23;
  auto tmp_5 = m22 * m13;
  auto tmp_6 = m02 * m33;
  auto tmp_7 = m32 * m03;
  auto tmp_8 = m02 * m23;
  auto tmp_9 = m22 * m03;
  auto tmp_10 = m02 * m13;
  auto tmp_11 = m12 * m03;
  auto tmp_12 = m20 * m31;
  auto tmp_13 = m30 * m21;
  auto tmp_14 = m10 * m31;
  auto tmp_15 = m30 * m11;
  auto tmp_16 = m10 * m21;
  auto tmp_17 = m20 * m11;
  auto tmp_18 = m00 * m31;
  auto tmp_19 = m30 * m01;
  auto tmp_20 = m00 * m21;
  auto tmp_21 = m20 * m01;
  auto tmp_22 = m00 * m11;
  auto tmp_23 = m10 * m01;

  auto t0 = tmp_0 * m11 + tmp_3 * m21 + tmp_4 * m31 -
            (tmp_1 * m11 + tmp_2 * m21 + tmp_5 * m31);
  auto t1 = tmp_1 * m01 + tmp_6 * m21 + tmp_9 * m31 -
            (tmp_0 * m01 + tmp_7 * m21 + tmp_8 * m31);
  auto t2 = tmp_2 * m01 + tmp_7 * m11 + tmp_10 * m31 -
            (tmp_3 * m01 + tmp_6 * m11 + tmp_11 * m31);
  auto t3 = tmp_5 * m01 + tmp_8 * m11 + tmp_11 * m21 -
            (tmp_4 * m01 + tmp_9 * m11 + tmp_10 * m21);

  auto d = 1.0 / (m00 * t0 + m10 * t1 + m20 * t2 + m30 * t3);

  res[0] = d * t0;
  res[1] = d * t1;
  res[2] = d * t2;
  res[3] = d * t3;
  res[4] = d * (tmp_1 * m10 + tmp_2 * m20 + tmp_5 * m30 -
                (tmp_0 * m10 + tmp_3 * m20 + tmp_4 * m30));
  res[5] = d * (tmp_0 * m00 + tmp_7 * m20 + tmp_8 * m30 -
                (tmp_1 * m00 + tmp_6 * m20 + tmp_9 * m30));
  res[6] = d * (tmp_3 * m00 + tmp_6 * m10 + tmp_11 * m30 -
                (tmp_2 * m00 + tmp_7 * m10 + tmp_10 * m30));
  res[7] = d * (tmp_4 * m00 + tmp_9 * m10 + tmp_10 * m20 -
                (tmp_5 * m00 + tmp_8 * m10 + tmp_11 * m20));
  res[8] = d * (tmp_12 * m13 + tmp_15 * m23 + tmp_16 * m33 -
                (tmp_13 * m13 + tmp_14 * m23 + tmp_17 * m33));
  res[9] = d * (tmp_13 * m03 + tmp_18 * m23 + tmp_21 * m33 -
                (tmp_12 * m03 + tmp_19 * m23 + tmp_20 * m33));
  res[10] = d * (tmp_14 * m03 + tmp_19 * m13 + tmp_22 * m33 -
                 (tmp_15 * m03 + tmp_18 * m13 + tmp_23 * m33));
  res[11] = d * (tmp_17 * m03 + tmp_20 * m13 + tmp_23 * m23 -
                 (tmp_16 * m03 + tmp_21 * m13 + tmp_22 * m23));
  res[12] = d * (tmp_14 * m22 + tmp_17 * m32 + tmp_13 * m12 -
                 (tmp_16 * m32 + tmp_12 * m12 + tmp_15 * m22));
  res[13] = d * (tmp_20 * m32 + tmp_12 * m02 + tmp_19 * m22 -
                 (tmp_18 * m22 + tmp_21 * m32 + tmp_13 * m02));
  res[14] = d * (tmp_18 * m12 + tmp_23 * m32 + tmp_15 * m02 -
                 (tmp_22 * m32 + tmp_14 * m02 + tmp_19 * m12));
  res[15] = d * (tmp_22 * m22 + tmp_16 * m02 + tmp_21 * m12 -
                 (tmp_20 * m12 + tmp_23 * m22 + tmp_17 * m02));

  return res;
}

/**
 * Multiply by an x rotation matrix
 * this is the optimized version of
 * return multiply(m, xRotation(angle));
 */
auto xRotate(Matrix4 m, Radians angle) -> Matrix4 {
  Matrix4 res;

  auto m10 = m[4];
  auto m11 = m[5];
  auto m12 = m[6];
  auto m13 = m[7];
  auto m20 = m[8];
  auto m21 = m[9];
  auto m22 = m[10];
  auto m23 = m[11];
  auto c = std::cos(angle);
  auto s = std::sin(angle);

  res[4] = c * m10 + s * m20;
  res[5] = c * m11 + s * m21;
  res[6] = c * m12 + s * m22;
  res[7] = c * m13 + s * m23;
  res[8] = c * m20 - s * m10;
  res[9] = c * m21 - s * m11;
  res[10] = c * m22 - s * m12;
  res[11] = c * m23 - s * m13;

  if (m != res) {
    res[0] = m[0];
    res[1] = m[1];
    res[2] = m[2];
    res[3] = m[3];
    res[12] = m[12];
    res[13] = m[13];
    res[14] = m[14];
    res[15] = m[15];
  }

  return res;
}

/**
 * Multiply by an y rotation matrix
 * this is the optimized version of
 * return multiply(m, yRotation(angle));
 */
auto yRotate(Matrix4 m, Radians angle) -> Matrix4 {
  Matrix4 res;

  auto m00 = m[0 * 4 + 0];
  auto m01 = m[0 * 4 + 1];
  auto m02 = m[0 * 4 + 2];
  auto m03 = m[0 * 4 + 3];
  auto m20 = m[2 * 4 + 0];
  auto m21 = m[2 * 4 + 1];
  auto m22 = m[2 * 4 + 2];
  auto m23 = m[2 * 4 + 3];
  auto c = std::cos(angle);
  auto s = std::sin(angle);

  res[0] = c * m00 - s * m20;
  res[1] = c * m01 - s * m21;
  res[2] = c * m02 - s * m22;
  res[3] = c * m03 - s * m23;
  res[8] = c * m20 + s * m00;
  res[9] = c * m21 + s * m01;
  res[10] = c * m22 + s * m02;
  res[11] = c * m23 + s * m03;

  if (m != res) {
    res[4] = m[4];
    res[5] = m[5];
    res[6] = m[6];
    res[7] = m[7];
    res[12] = m[12];
    res[13] = m[13];
    res[14] = m[14];
    res[15] = m[15];
  }

  return res;
}

/**
 * Computes a 4-by-4 perspective transformation matrix given the angular height
 * of the frustum, the aspect ratio, and the near and far clipping planes.  The
 * arguments define a frustum extending in the negative z direction.  The given
 * angle is the vertical angle of the frustum, and the horizontal angle is
 * determined to produce the given aspect ratio.  The arguments near and far are
 * the distances to the near and far clipping planes.  Note that near and far
 * are not z coordinates, but rather they are distances along the negative
 * z-axis.  The matrix generated sends the viewing frustum to the unit box.
 * We assume a unit box extending from -1 to 1 in the x and y dimensions and
 * from -1 to 1 in the z dimension.
 * @param field_of_view - field of view in y axis.
 * @param aspect - aspect of viewport (width / height)
 * @param near - near Z clipping plane
 * @param far - far Z clipping plane
 */
auto perspective(Radians field_of_view, float aspect, float near, float far)
    -> Matrix4 {
  Matrix4 res;

  auto f = std::tan(M_PI * 0.5 - 0.5 * field_of_view);
  auto rangeInv = 1.0 / (near - far);

  res[0] = f / aspect;
  res[1] = 0;
  res[2] = 0;
  res[3] = 0;
  res[4] = 0;
  res[5] = f;
  res[6] = 0;
  res[7] = 0;
  res[8] = 0;
  res[9] = 0;
  res[10] = (near + far) * rangeInv;
  res[11] = -1;
  res[12] = 0;
  res[13] = 0;
  res[14] = near * far * rangeInv * 2;
  res[15] = 0;

  return res;
}

auto getAngleBetweenVectors(const Point3D& a, const Point3D& b) -> Radians {
  auto dot_product = a.x * b.x + a.y * b.y + a.z * b.z;

  auto a_len = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
  auto b_len = std::sqrt(b.x * b.x + b.y * b.y + b.z * b.z);
  auto len_mult = a_len * b_len;

  auto angle_cos = dot_product / len_mult;

  return std::acos(angle_cos);
}
