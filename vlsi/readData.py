import csv


class DataSet:

	def __init__(self, filename):

		self.label = []
		self.rawData = []
		self.filename = filename
		self.dataSize = 0
		self.inputDataDimension = 0
		self.data = []

	def readFromFile(self):

		f = open(self.filename,'r')
		csvReader = csv.reader(f)

		for row in csvReader:
			
			#remove ID
			del row[0]
			#store DATA LABEL 
			if self.dataSize == 0:

				self.label.append(row)
				self.inputDataDimension = len(self.label[0])
			else:
				self.rawData.append(row)
			self.dataSize +=1

		f.close()
		self.dataSize -=1 # exclude label line


	def printData(self):

		print self.rawData[0][0]
		print ""
		print "dataSize = ", self.dataSize
		print "dataDimension = ", self.inputDataDimension


	def convertDataType(self):
		data = []
		for row in range(0,self.inputDataDimension):
		 	tempData = []
		 	tupleDict = {}
		 	tupleNum = 0
		 	for col in range(0,self.dataSize):
		 		#print "a"
		 		
		 		d = self.rawData[col][row]
		 		

		 		#number
		 		if checkNumber(d):

		 			num = float(d)
		 			tempData.append(num)
		 		#not a number
		 		else:
		 			if d not in tupleDict :
		 				tupleDict[d] = tupleNum
		 				tupleNum+=1
		 				


		 	data.append(tempData)

		 #print data[0]




def checkNumber(str):
	try:
		float(str)
		return True 
	except ValueError:
		return False