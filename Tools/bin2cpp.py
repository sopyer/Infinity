import sys, struct

bytesInRow = 30

inputFile = "anoprob.ttf"

file = open(inputFile, "rb");
data = file.read()

print "static uint8_t rawData = {"
str = "\t"
i=0
for  i in xrange(len(data)):
	str += hex(struct.unpack_from("B", data, i)[0])+","
	if ((i%bytesInRow)==(bytesInRow-1)) and i>0:
		print str
		str = "\t"
if str:
	print str
print "};"

file.close()

print inputFile
