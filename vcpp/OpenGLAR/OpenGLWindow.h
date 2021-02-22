#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

void CreateOpenGLWindow(void); 
void RefreshOpenGLWindow(void);

void GetModelviewMatrixAftergluLookAt(double *mat);
void GetPerspectiveParameters(double &fovy, double &aspect, double &zNear, double &zFar);

#endif