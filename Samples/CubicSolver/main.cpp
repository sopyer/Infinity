#include <stdio.h>
#include <cmath>

/*
Driver program to demponstrate QUADROOTS, CUBICROOTS, BIQUADROOTS
*/
int BIQUADROOTS(double p[5], double r[3][5]);
int QUADROOTS(double p[5], double r[3][5]);
int CUBICROOTS(double p[5], double r[3][5]);

float area(float* v0, float* v1, float* v2)
{
	return 0.5f*((v1[0]-v0[0])*(v1[1]+v0[1]) + 
				 (v2[0]-v1[0])*(v2[1]+v1[1]) + 
				 (v0[0]-v2[0])*(v0[1]+v2[1]));
}

void calcBarycentric(/*vec2*/float* v0, /*vec2*/float* v1, /*vec2*/float* v2,/*vec2*/float* pt, /*vec3*/float* bary)
{
	float triArea = area(v0, v1, v2);
	bary[0] = area(v1, v2, pt)/triArea;
	bary[1] = area(v2, v0, pt)/triArea;
	bary[2] = area(v0, v1, pt)/triArea;
}

void solveCubic(float k[4], int& count, float r[3])
{
	float a=k[0], b=k[1], c=k[2], d=k[3];

	float u = (3*a*c-b*b)/*/a/a/3*/;
	float v = (27*a*a*d-9*a*b*c+2*b*b*b)/*/27/a/a/a*/;

	float d0 =  k[0];
	float d1 = -k[1]/3.0f;
	float d2 =  k[2]/3.0f;
	float d3 = -k[3];

	float dt1 = d0*d2-d1*d1;//k[0]*k[2]-k[1]*k[1];
	float dt2 = d1*d2-d0*d3;//k[1]*k[2]-k[0]*k[3];
	float dt3 = d1*d3-d2*d2;//k[1]*k[3]-k[2]*k[2];

	float pu = -dt2*dt2;
	float pv = 4*dt1*dt3;

	float det = pv+pu;

	float j = 4*u*u*u+v*v;

	float ratio = det/j;
	float ratio2 = j/det;

	if (j<=0)
	{
		float s = sqrt(-u);
	
		float cs=0, sn=0;

		if (s!=0)
		{
			float t = -v/2/s/s/s;
			float ang = acos(t)/3;

			cs = cos(ang);
			sn = sqrt(3.0f)*sin(ang);
		}

		count = 3;

		r[0] = (2*s*cs-b)/3/a;
		r[1] = (s*(-cs+sn)-b)/3/a;
		r[2] = (s*(-cs-sn)-b)/3/a;
	}
	else
	{
		float w = sqrt(j);
		float wv2 = (w+abs(v))/2.0f;

		wv2 = pow (wv2, 1.0f/3.0f);

		count = 1;
		r[0] = ((v<0?-1:1)*(u/wv2-wv2)-b)/3/a;
	}
}

void main()
{
	float v0[2] = {0.0f, 1.0f};
	float v1[2] = {1.0f, 0.0f};
	float v2[2] = {0.0f, 0.0f};
	float pt[2] = {1.5f, 100.5f};
	float bary[3];

	calcBarycentric(v0, v1, v2, pt, bary);

	printf("%f %f, %f, %f\n", area(v0, v1, v2), bary[0], bary[1], bary[2]);

	printf("%f %f\n", pt[0], pt[1]);
	printf("%f %f\n", v0[0]*bary[0]+v1[0]*bary[1]+v2[0]*bary[2],
					  v0[1]*bary[0]+v1[1]*bary[1]+v2[1]*bary[2]);


	//float kk[] = {2, -4, -22, 24};
	//float kk[] = {1, -6, 9, -4};
	//float kk[] = {1, 6, 12, 8};
	float kk[] = {3, -10, 14, 27};

	float rr[3];
	int ccc;
	solveCubic(kk, ccc, rr);

  int i,k,n;
  double p[5],r[3][5];

  for (n=2; n<=4; n++){
/* Various test cases */
  if(n==4)
  { p[0]=1; p[1]=-10; p[2]=35; p[3]=-50; p[4]=24; /* 1,2,3,4 */
    p[0]=1; p[1]=0; p[2]=4; p[3]=0; p[4]=4; /* +-i sqrt(2) */
  }
  if(n==3) { p[0]=1; p[1]=-6; p[2]=11; p[3]=-6; } /* 1,2,3 */
  if(n==2) { p[0]=1; p[1]=-2; p[2]=2;} /* 1+- i */

  if(n==2){
    printf("Testing quadratic\n");
    i=QUADROOTS(p,r);
  }
  else if(n==3){
    printf("\nTesting cubic\n");
    i=CUBICROOTS(p,r);
  }
  else if(n==4){
    printf("\nTesting biquadratic\n");
    i=BIQUADROOTS(p,r);
  }

  for(k=1;k<=n;k++)printf(" Re=%14.10f Im=%14.10f \n",r[1][k],r[2][k]);
 }
}

/* CACM Algorithm 326
   Roots of low order polynomials
   Author: Terence R.F.Nonweiler
   CACM  (Apr 1968) p269
   Translated into c and programmed by M.Dow
   ANUSF, Australian National University, Canberra, Australia
   m.dow@anu.edu.au

Suite of procedures for finding the (complex) roots of the
quadratic, cubic or quartic polynomials by explicit algebraic methods.
Each Returns
x=r[1][k] + i r[2][k]  k=1,...,n, where n={2,3,4}
as roots of
sum_{k=0:n} p[k] x^(n-k) =0
Assume p[0]<>0 (overflows otherwise)
*/
#include <stdio.h>
#include <math.h>

/*
Array r[3][5]  p[5]
Roots of poly p[0] x^2 + p[1] x+p[2]=0
x=r[1][k] + i r[2][k]  k=1,2
*/
int QUADROOTS(double p[5], double r[3][5])
{
  double b,c,d;
  b=-p[1]/p[0]/2;
  c=p[2]/p[0];
  d=b*b-c;
  if(d>0)
  {
    if(b>0) b=r[1][2]=sqrt(d)+b;
    else    b=r[1][2]=-sqrt(d)+b;
    r[1][1]=c/b; r[2][1]=r[2][2]=0;
  }
  else
  {
    d=r[2][1]=sqrt(-d); r[2][2]=-d;
    r[1][1]=r[1][2]=b;
  }
  return(0);
}

/*
Array r[3][5]  p[5]
Roots of poly p[0] x^3 + p[1] x^2...+p[3]=0
x=r[1][k] + i r[2][k]  k=1,...,3
Assumes 0<arctan(x)<pi/2 for x>0
*/
int CUBICROOTS(double p[5], double r[3][5])
{
  double s,t,b,c,d;
  int k;
  if(p[0]!=1)
   for(k=1;k<4;k++) p[k]=p[k]/p[0]; p[0]=1;
   s=p[1]/3.0; t=s*p[1];
   b=0.5*(s*(t/1.5-p[2])+p[3]); t=(t-p[2])/3.0;
   c=t*t*t; d=b*b-c;
   if(d>=0)
   {
     d=pow((sqrt(d)+fabs(b)),1.0/3.0);
     printf("d=%f\n",d);
     if(d!=0)
     {
       if(b>0) b=-d;
       else b=d;
       c=t/b;
     }
     d=r[2][2]=sqrt(0.75)*(b-c); b=b+c;
     c=r[1][2]=-0.5*b-s;
     if((b>0 && s<=0) || (b<0 && s>0))
     {
       r[1][1]=c; r[2][1]=-d; r[1][3]=b-s;
       r[2][3]=0;
     }
     else
     {
       r[1][1]=b-s; r[2][1]=0; r[1][3]=c;
       r[2][3]=-d;
     }
   }  /* end 2 equal or complex roots */
   else
   {
     if(b==0)
       d=atan(1.0)/1.5;
     else
       d=atan(sqrt(-d)/fabs(b))/3.0;
     if(b<0)
       b=sqrt(t)*2.0;
     else
       b=-2.0*sqrt(t);
     c=cos(d)*b; t=-sqrt(0.75)*sin(d)*b-0.5*c;
     d=-t-c-s; c=c-s; t=t-s;
     if(fabs(c)>fabs(t))
       r[1][3]=c;
     else
       {
       r[1][3]=t; t=c;
       }
     if(fabs(d)>fabs(t))
       r[1][2]=d;
     else
     {
       r[1][2]=t; t=d;
     }
     r[1][1]=t;
     for(k=1;k<4;k++) r[2][k]=0;
   }
   return(0);
}

/*
Array r[3][5]  p[5]
Roots of poly p[0] x^4 + p[1] x^3...+p[4]=0
x=r[1][k] + i r[2][k]  k=1,...,4
*/
int BIQUADROOTS(double p[5], double r[3][5])
{
  double a,b,c,d,e;
  int k,j;
  if(p[0] != 1.0)
  {
    for(k=1;k<5;k++) p[k]=p[k]/p[0];
    p[0]=1;
  }
  e=0.25*p[1];
  b=2*e;
  c=b*b;
  d=0.75*c;
  b=p[3]+b*(c-p[2]);
  a=p[2]-d;
  c=p[4]+e*(e*a-p[3]);
  a=a-d;
  p[1]=0.5*a;
  p[2]=(p[1]*p[1]-c)*0.25;
  p[3]=b*b/(-64.0);
  if(p[3]<-1e-6)
  {
    CUBICROOTS(p,r);
    for(k=1;k<4;k++)
    {
      if(r[2][k]==0 && r[1][k]>0)
      {
	d=r[1][k]*4; a=a+d;
	if(a>=0 && b>=0)
	  p[1]=sqrt(d);
	else if(a<=0 && b<=0)
	  p[1]=sqrt(d);
	else p[1]=-sqrt(d);
	b=0.5*(a+b/p[1]);
	goto QUAD;
      }
    }
  }
  if(p[2]<0)
  {
    b=sqrt(c); d=b+b-a;
    p[1]=0; if(d>0) p[1]=sqrt(d);
  }
  else
  {
    if(p[1]>0)
      b=sqrt(p[2])*2.0+p[1];
    else
      b=-sqrt(p[2])*2.0+p[1];
    if(b!=0)
      p[1]=0;
    else
      {
	for(k=1;k<5;k++)
	{
	  r[1][k]=-e;
	  r[2][k]=0;
	}
	goto END;
      }
    }
QUAD:p[2]=c/b; QUADROOTS(p,r);
  for(k=1;k<3;k++)
  for(j=1;j<3;j++) r[j][k+2]=r[j][k];
  p[1]=-p[1]; p[2]=b; QUADROOTS(p,r);
  for(k=1;k<5;k++) r[1][k]=r[1][k]-e;
  END:;
  return(0);
}
