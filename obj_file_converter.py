from random import random 

def convert(inputFile, outputFile):
	obj_file = open(inputFile, 'r')
	new_file = open(outputFile, 'w')
	obj_file = list(obj_file.read().split('\n'))

	#keep track of index and file line number
	lineNum = 0
	labelsNum = 0

	valid_operations = {"v", "f", "Va", "Vd", "Vs"}

	output = ""
	#camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
	output += "size 640 480\n"
	#output += "camera -20 -20 -20 20 20 25 0 0 1 45\n"
	output += "camera TO_BE_REPLACED TO_BE_REPLACED TO_BE_REPLACED -TO_BE_REPLACED -TO_BE_REPLACED -TO_BE_REPLACED 0 0 1 45\n"
	

	output += "output " + outputFile[0:len(outputFile) - 5] + ".png\n"

	#output += "point 0 0 -5 1 1 1\n"
	#output += "point 0 0 5 1 1 1\n"
	#output += "point 0 -5 0 1 1 1\n"
	#output += "point 0 -10 0 1 1 1\n"
	#output += "point 0 10 0 1 1 1\n"
	#output += "point 10 0 0 1 1 1\n"
	#output += "point -10 0 0 1 1 1\n"
	#output += "directional 0 0 -10 1 1 1\n"
	#output += "point 0 0 10 1 1 1\n"
	
	output += "ambient 1 0 1\n"
	largest_disance = 0

	for line in obj_file:
		instr = line.split(); #split to get instruction and different operands
		if len(instr) >= 1:
			if instr[0] in valid_operations:
				if instr[0] == "v":
					output += "vertex "
					output += instr[1] + " " + instr[2] + " " + instr[3]
					largest_disance = max(abs(float(instr[1])), abs(float(instr[1])), abs(float(instr[1])))
				elif instr[0] == "f":
					#for i in range(1, len(instr)):
						#if (i + 2) < len(instr) and (i - 1) >= 0:
							#output += "tri "
							#output += str(int(instr[i]) - 1) + " " + str(int(instr[i + 1]) - 1) + " " + str(int(instr[i + 2]) - 1) + "\n"
					starting_point = int(instr[1]) - 1
					for j in range(2, len(instr)):
						if (j + 1) < len(instr):
							output += "tri "
							output += str(starting_point) + " " + str(int(instr[j]) - 1) + " " + str(int(instr[j + 1]) - 1) + "\n"
				elif instr[0] == "Va":
					output += "ambient "
					output += instr[1] + " " + instr[2] + " " + instr[3]
				elif instr[0] == "Vd":
					output += "diffuse "
					output += instr[1] + " " + instr[2] + " " + instr[3]
				elif instr[0] == "Vs":
					output += "specular "
					output += instr[1] + " " + instr[2] + " " + instr[3]	
				output += "\n"
		intensity_x = random()
		intensity_y = random()
		intensity_z = random()
		output += "ambient " + str(intensity_x) + " " + str(intensity_y) + " " + str(intensity_z) +"\n"
	output = output.replace("TO_BE_REPLACED", str(largest_disance * 3))
	new_file.write(str(output) + '\n')
	new_file.close()

inputFileName = input("Name of object file (in directory): \n")
outputFileName = inputFileName[0:len(inputFileName) - 4] + "_converted.test"
convert(inputFileName, outputFileName)