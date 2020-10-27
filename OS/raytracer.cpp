#include "raytracer.h"

float minDist = 0.003162277;
//float minDist = 0.01;
float MaxDistance = 1000.0;
float eps = minDist;

int MaxRaySteps = 50;

double camera_fov = 1.5;
double camera_aspect = 320.0 / 200.0;


float DE(Vector pos) {
	return ((float)pos.length() - 1.0f);
}


float DEBox(Vector pos) {
	pos.x = dmax(dabs(pos.x) - 1.0, 0.0);
	pos.y = dmax(dabs(pos.y) - 1.0, 0.0);
	pos.z = dmax(dabs(pos.z) - 1.0, 0.0);
	
	return pos.length();
}

int Iterations = 5;
float Scale = 3.0;
Vector Offset(1.0, 1.0, 1.0);


float DEMenger(Vector pos) {
	Vector pos0(pos.x, pos.y, pos.z);
	
	int n = 0;
	while (n < Iterations) {
		pos.x = dabs(pos.x);
		pos.y = dabs(pos.y);
		pos.z = dabs(pos.z);
		
		
		pos0.x = pos.x; pos0.y = pos.y; pos0.z = pos.z;
		if (pos.x < pos.y) {
			pos.x = pos0.y;
			pos.y = pos0.x;
		}
		pos0.x = pos.x; pos0.y = pos.y; pos0.z = pos.z;
		if (pos.x < pos.z) {
			pos.x = pos0.z;
			pos.z = pos0.x;
		}
		pos0.x = pos.x; pos0.y = pos.y; pos0.z = pos.z;
		if (pos.y < pos.z) {
			pos.y = pos0.z;
			pos.z = pos0.y;
		}
		
		//pos = (pos * Scale) - Offset*(Scale-1.0);
		pos.x = (pos.x * Scale) - (Scale - 1.0);
		pos.y = (pos.y * Scale) - (Scale - 1.0);
		pos.z = (pos.z * Scale) - (Scale - 1.0);
		
		
		if (pos.z < -1.0) {
			pos.z += 2.0;
		}
		
		//if (pos.z < -0.5*Offset.z*(Scale-1.0)) {
		//	pos.z += Offset.z*(Scale-1.0);
		//}
		
		n++;
	}
	
	return (((float)pos.length() - 1.5) * (float)(1.0 / pow((double)Scale, (double)n)));
}

Vector normal(Vector pos, float normalDistance) {
	normalDistance = dmax(normalDistance*0.5, 1.0e-7);
	Vector e(0.0,normalDistance,0.0);
	Vector n(DE(pos+e.yxx())-DE(pos-e.yxx()),
		DE(pos+e.xyx())-DE(pos-e.xyx()),
		DE(pos+e.xxy())-DE(pos-e.xxy()));
	n = n.norm();
	return n;
}

float lighting(Vector n, float color, Vector pos, Vector dir, float eps) {
	
}


float trace(Vector from, Vector dir) {
	float dist = 0.0;
	float totalDist = 0.0;
	
	float epsModified = 0.0;
	
	Vector direction = dir.norm();
	
	int steps;
	for (steps=0; steps<MaxRaySteps; steps++) {
		Vector p = (from + (direction * totalDist));
		dist = DEMenger(p);

		totalDist += dist;
		epsModified = pow(totalDist, 1.0)*eps;
		if (dist < epsModified) {
			totalDist -= (epsModified-dist);
			break;
		}
		if (totalDist > MaxDistance) {
			break;
		}
	}
	
	return totalDist;
}

void render(Vector camera_eye, Vector camera_target, Vector camera_up) {
	Vector ww = (camera_target - camera_eye).norm();
	Vector uu = (ww.cross(camera_up)).norm();
	Vector vv = (uu.cross(ww)).norm();
	
	double look_at_x[] = {uu.x, vv.x, ww.x,  uu.y, vv.y, ww.y,  uu.z, vv.z, ww.z,  0, 0, 0, 0, 0, 0, 0};
	Matrix look_at(3, 3, look_at_x);
	
	for (int x = 0; x < 320; x++) {
		for (int y = 0; y < 200; y++) {
			double x_norm = (((double)x * 2.0) / 320.0) - 1.0;
			double y_norm = (((double)y * 2.0) / 200.0) - 1.0;
			
			double pix_norm_x[] = {x_norm, y_norm / camera_aspect, camera_fov,  0, 0, 0,  0, 0, 0,  0, 0, 0, 0, 0, 0, 0};
			Matrix pix_norm(1, 3, pix_norm_x);
			
			Vector camera_ray = (look_at * pix_norm).get_vector();
			
			float dist = trace(camera_eye, camera_ray);
			
			if (dist < 1000.0) {
				
				//int brightness = (int)((dist / 1000.0) * 255.0);
				int brightness = (int)(dmin(dmax(20.0 - dist, 0.0), 20.0) * (255.0 / 20.0));
				
				set_pixel(x, y, brightness);
			} else {
				set_pixel(x, y, 0);
			}
		}
		flush();
	}

	//camera_ray = normalize(camera_lookat* vec3(VertexPosition.x, VertexPosition.y / camera_aspect, camera_fov));
	
}