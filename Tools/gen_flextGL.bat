python flextGL\flextGLgen.py -D generated -t templates\opengl flextGL.in
copy generated\opengl.h ..\SDK\include\
copy generated\opengl.c ..\SDK\gfx\
