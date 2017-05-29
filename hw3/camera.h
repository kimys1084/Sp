
#pragma once


class Camera {
public:
  Camera ();
  virtual ~Camera () {}

  inline float getFovAngle () const { return fovAngle; }
  inline void setFovAngle (float newFovAngle) { fovAngle = newFovAngle; }
  inline float getAspectRatio () const { return aspectRatio; }
  inline float getNearPlane () const { return nearPlane; }
  inline void setNearPlane (float newNearPlane) { nearPlane = newNearPlane; }
  inline float getFarPlane () const { return farPlane; }
  inline void setFarPlane (float newFarPlane) { farPlane = newFarPlane; }
  inline unsigned int getScreenWidth () const { return W; }
  inline unsigned int getScreenHeight () const { return H; }

  void resize (int W, int H);

  void move (float dx, float dy, float dz);
  void beginRotate (int u, int v);
  void rotate (int u, int v);
  void endRotate ();
  void zoom (float z);
  void apply ();

  void setScale( float r, float t, float z ) {rot_scale=r; trans_scale=t; zoom_scale=z; }

private:
  float fovAngle;
  float aspectRatio;
  float nearPlane;
  float farPlane;

  int moving;
  int beginu, beginv;
  int H, W;
  float curquat[4];
  float lastquat[4];
  float x, y, z;
  float dolly;

  float rot_scale;
  float trans_scale;
  float zoom_scale;
};
