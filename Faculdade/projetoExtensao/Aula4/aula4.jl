using DecisionTree

using Random, Statistics

X, y = load_data("iris")

X = float.(X)
y = string.(y)

iris = [X y]

function perclass_split(y, percent)
    uniq_class = unique(y)
    keep_index = []
    for class in uniq_class
            class_index = findall(y .== class)
            row_index =  randsubseq(class_index, percent)
            push!(keep_index, row_index...)
    end
    return keep_index
end

Random.seed!(1)

train_index = perclass_split(y, 0.67)

test_index = setdiff(1:length(y), train_index)

X_train = X[train_index, :]

X_test = X[test_index, :]

y_train = y[train_index]

y_test = y[test_index]
    
model = DecisionTreeClassifier(max_depth = 2)

fit!(model, X_train, y_train)

print_tree(model)

train = [X_train y_train]

y_hat = predict(model, X_test)

accuracy = mean(y_hat .== y_test)

DecisionTree.confusion_matrix(y_test, y_hat)

check = [y_hat[i] == y_test[i] for i in 1:length(y_hat)]
check_display = [y_hat y_test check]

vscodedisplay(check_display)

prob = predict_proba(model, X_test)

vscodedisplay(prob)



# RAndom Tree (Bagging)

model = RandomForestClassifier(n_trees=20)

fit!(model, X_train, y_train)

y_hat = predict(model, X_test)

accuracy = mean(y_hat .== y_test)

DecisionTree.confusion_matrix(y_test, y_hat)

check = [y_hat[i] == y_test[i] for i in 1:length(y_hat)]
check_display = [y_hat y_test check]

vscodedisplay(check_display)

prob = predict_proba(model, X_test)

vscodedisplay(prob)


# AdaBoost (Boosting)

model = adaBoostStumpClassifier(x_test, y_test)

fit!(model, X_train, y_train)

y_hat = predict(model, X_test)

accuracy = mean(y_hat .== y_test)

DecisionTree.confusion_matrix(y_test, y_hat)

check = [y_hat[i] == y_test[i] for i in 1:length(y_hat)]
check_display = [y_hat y_test check]

vscodedisplay(check_display)

prob = predict_proba(model, X_test)

vscodedisplay(prob)
