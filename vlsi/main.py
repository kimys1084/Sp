from readData import DataSet, checkNumber
filePath = 'Data/ml_project_train.csv'

trainData = DataSet(filePath)

trainData.readFromFile()
trainData.printData()

trainData.convertDataType()


#print checkNumber("123123as")
