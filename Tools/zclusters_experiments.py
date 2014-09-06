import math

def geomN(b, q, n):
    return b*(q**n)

def geomSum(b, q, n):
    return b*(q**n - 1) / (q - 1)

numClusters = 128

b=4
q=1.01

width  = 1280
height = 720
fov = 30 * math.pi / 180
aspect = 1.6
zfar = 1000.0
znear = 0.1
yscale = math.tan(0.5 * fov)
xscale = yscale*aspect

#~ print(xscale, yscale)

#
# Experiment 1
#

#~ for i in range(numClusters):
    #~ d = geomN  (b, q, i)
    #~ z = geomSum(b, q, i)
    #~ dx = z*xscale
    #~ dy = z*yscale
    #~ print(d, dx, dy, z)



#
# Experiment 2
#

#~ zLogScale = numClusters / math.log(zfar / znear)
#~ sc        = 1.0 #1.0/40.0
#~ zscale    = -1.0 / znear * sc
#~ zoffset   = znear*(1.0-sc)
#~ for i in range(numClusters):
    #~ depth = -(math.exp(float(i) / zLogScale) - zoffset)/zscale
    #~ print(depth)

#~ viewSpaceZ = -1000
#~ print (math.log(viewSpaceZ * zscale + zoffset) * zLogScale)



#
# Experiment 3
#

numClusters = 32
q = 1.116;
zLogScale = 1.0/math.log(q)
zscale    = -(q-1)/b
zoffset   = 1.0
for i in range(numClusters):
    depth = -(math.exp(float(i) / zLogScale) - zoffset)/zscale
    print(depth)

viewSpaceZ = -1000
print (math.log(viewSpaceZ * zscale + zoffset) * zLogScale)


#
# Experiment 4
#

#~ b=4
#~ q = math.pow(zfar*(q-1)/b+1.0, 1.0/numClusters)
#~ print(q)
#~ zLogScale = 1.0/math.log(q)
#~ zscale    = -(q-1)/b
#~ zoffset   = 1.0
#~ for i in range(numClusters):
    #~ depth = -(math.exp(float(i) / zLogScale) - zoffset)/zscale
    #~ print(depth)

#~ viewSpaceZ = -1000
#~ print (math.log(viewSpaceZ * zscale + zoffset) * zLogScale)



#
# Experiment 5
#

#~ grid2dDimY = (height + 63) / 64;
#~ sD = 2.0 * math.tan(fov * 0.5) / float(grid2dDimY);
#~ zGridLocFar = math.log(zfar / znear)/math.log(1.0 + sD);
#~ recNear = 1.0 / znear;
#~ numClusters = int(math.ceil(zGridLocFar) + 0.5)
#~ recLogSD1 = 1.0 / math.log( sD + 1.0 );

#~ print (numClusters)

#~ zLogScale = recLogSD1
#~ zscale    = -recNear
#~ zoffset   = 0.0
#~ for i in range(numClusters):
    #~ depth = -(math.exp(float(i) / zLogScale) - zoffset)/zscale
    #~ print(depth)

#~ viewSpaceZ = -1000
#~ print (math.log(viewSpaceZ * zscale + zoffset) * zLogScale)

