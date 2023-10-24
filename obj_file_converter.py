from random import random 

def open_mtl_file(mtl_filename):
	mtl_file = open(mtl_filename, 'r')

	valid_operations = {"Kd", "Ks", "Ka", "newmtl", "illum", "Ns"}

	material_dictionary = {}
	current_material = ""
	for line in mtl_file:
		instr = line.split()
		if len(instr) >= 1:
			output = ""
			if instr[0] == "newmtl":
				current_material = instr[1]
			if instr[0] == "Kd":
				output += "diffuse "
				output += instr[1] + " " + instr[2] + " " + instr[3] + "\n"
			elif instr[0] == "Ks":
				output += "specular "
				output += instr[1] + " " + instr[2] + " " + instr[3] + "\n"
			elif instr[0] == "Ka":
				output += "ambient "
				output += instr[1] + " " + instr[2] + " " + instr[3] + "\n"
			elif instr[0] == "Ns":
				output += "shininess "
				output += instr[1] + "\n"	
			if current_material in material_dictionary.keys():
				material_dictionary[current_material] += output
			else:
				material_dictionary[current_material] = output
	return material_dictionary

def convert(inputFile, outputFile):
	obj_file = open(inputFile, 'r')
	new_file = open(outputFile, 'w')
	obj_file = list(obj_file.read().split('\n'))

	material_dictionary = {}

	valid_operations = {"v", "f", "mtllib", "usemtl"}

	output = ""
	output += "size 640 480\n"
	#camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
	output += "camera TO_BE_REPLACED TO_BE_REPLACED TO_BE_REPLACED -TO_BE_REPLACED -TO_BE_REPLACED -TO_BE_REPLACED 0 0 1 45\n"
	

	output += "output " + outputFile[0:len(outputFile) - 5] + ".png\n"

	#output += "ambient 1 0 1\n"
	largest_distance = 0

	for line in obj_file:
		instr = line.split(); #split to get instruction and different operands
		if len(instr) >= 1:
			if instr[0] in valid_operations:
				if instr[0] == "v":
					output += "vertex "
					output += instr[1] + " " + instr[2] + " " + instr[3]
					largest_distance = max(largest_distance, abs(float(instr[1])), abs(float(instr[1])), abs(float(instr[1])))
				elif instr[0] == "f":
					if "/" not in instr[1] and "//" not in instr[1]:	
						starting_point = int(instr[1]) - 1
						for j in range(2, len(instr)):
							if (j + 1) < len(instr):
								output += "tri "
								output += str(starting_point) + " " + str(int(instr[j]) - 1) + " " + str(int(instr[j + 1]) - 1) + "\n"
				elif instr[0] == "mtllib":
					mtl_filename = instr[1] 
					material_dictionary = open_mtl_file(mtl_filename)
				elif instr[0] == "usemtl":
					if instr[1] in material_dictionary.keys():
						output += material_dictionary[instr[1]]
				elif instr[0] == "Ka":
					output += "ambient "
					output += instr[1] + " " + instr[2] + " " + instr[3] + "\n"
				elif instr[0] == "Kd":
					output += "diffuse "
					output += instr[1] + " " + instr[2] + " " + instr[3] + "\n"
				elif instr[0] == "Ks":
					output += "specular "
					output += instr[1] + " " + instr[2] + " " + instr[3] + "\n"
				output += "\n"
		intensity_x = random()
		intensity_y = random()
		intensity_z = random()
		output += "ambient " + str(intensity_x) + " " + str(intensity_y) + " " + str(intensity_z) +"\n"
	output = output.replace("TO_BE_REPLACED", str(largest_distance * 1.5))
	new_file.write(str(output) + '\n')
	new_file.close()

inputFileName = input("Name of object file (in directory): \n")
outputFileName = inputFileName[0:len(inputFileName) - 4] + "_converted.test"
convert(inputFileName, outputFileName)