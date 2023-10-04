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
	output += "output " + outputFile[0:len(outputFile) - 5] + ".png\n"
	output += "point 0 0 0 1 1 1\n"
	output += "camera 0 0 0 10 0 0 0 0 1 45\n"
	
	for line in obj_file:
		instr = line.split(); #split to get instruction and different operands
		if len(instr) >= 1:
			if instr[0] in valid_operations:
				if instr[0] == "v":
					output += "vertex "
					output += instr[1] + " " + instr[2] + " " + instr[3]
				elif instr[0] == "f":
					for i in range(1, len(instr)):
						if (i + 2) < len(instr):
							output += "tri "
							output += instr[i] + " " + instr[i + 1] + " " + instr[i + 2] + "\n"
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
	new_file.write(str(output) + '\n')
	new_file.close()

inputFileName = input("Name of object file (in directory): \n")
outputFileName = inputFileName[0:len(inputFileName) - 4] + "_converted.test"
convert(inputFileName, outputFileName)