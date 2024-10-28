using DecisionTree, CSV

train_data = CSV.File("/home/guilherme/github/ovomaltine/Faculdade/projetoExtensao/DIAFINAL/train.csv")
test_data = CSV.File("/home/guilherme/github/ovomaltine/Faculdade/projetoExtensao/DIAFINAL/test.csv")

train_data["PassengerId"]
