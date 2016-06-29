# load adabag library

library("adabag")
library("rpart")

# load iris dataset

data("iris")

# training dataset

train.index <- c(sample(1:50, 25),
                 sample(51:100, 25),
                 sample(100:150, 25))
iris.train <- iris[train.index, ]

# train adaboost model

iris.adaboost <- boosting(formula = Species ~ .,
                          data = iris.train,
                          mfinal = 10,
                          control = rpart.control(maxdepth = 1))

# plotting barplot of relative importance of training features

barplot(iris.adaboost$importance[order(iris.adaboost$importance,
                                       decreasing = TRUE)],
        ylim = c(0, 100),
        main = "Variables Relative Importance",
        col = "lightblue")

# print the confusion matrix

print(table(iris.adaboost$class,
            iris.train[, "Species"],
            dnn = c("Predicted Class",
                    "Observed Class")))

# print misclassification error

iris.adaboost.me = 1 - sum(iris.adaboost$class == iris.train$Species) / length(iris.train$Species)

print(iris.adaboost.me)

# get test data

iris.test <- iris[-train.index, ]

# predict on new test examples

iris.adaboost.predict <- predict.boosting(iris.adaboost,
                                          iris.test)

# print the confusion matrix

print(iris.adaboost.predict$confusion)

# print misclassification error

print(iris.adaboost.predict$error)

# apply 10-fold cross-validation on adaboost model

iris.adaboost.cv <- boosting.cv(formula = Species ~ .,
                                v = 10,
                                data = iris,
                                mfinal = 10,
                                control = rpart.control(maxdepth = 1))

# print the confusion matrix

print(iris.adaboost.cv$confusion)

# print misclassification error

print(iris.adaboost.cv$error)

# train bagging model

iris.bagging <- bagging(Species ~ .,
                        data = iris.train,
                        mfinal = 10,
                        control = rpart.control(maxdepth = 1))

# barplot of relative importance of variables

barplot(iris.bagging$importance[order(iris.bagging$importance,
                                      decreasing = TRUE)],
        ylim = c(0, 100),
        main = "Variables of Relative Importance",
        col = "lightblue")

# confusion matrix

print(table(iris.bagging$class,
            iris.train$Species,
            dnn = c("Predicted Class", "Observed Class")))

iris.bagging.me = 1 - sum(iris.bagging$class == iris.train$Species) / length(iris.train$Species)

print(iris.bagging.me)


iris.bagging.predict <- predict.bagging(iris.bagging,
                                        newdata = iris.test)

print(iris.bagging.predict$confusion)

print(iris.bagging.predict$error)

iris.bagging.margins <- margins(iris.bagging, iris.train)
iris.bagging.margins.predict <- margins(iris.bagging.predict, iris.train)
margins.train <- iris.bagging.margins$margins
margins.test <- iris.bagging.margins.predict$margins

plot(sort(margins.train),
     1:length(margins.train)/length(margins.train),
     type = "l",
     xlim = c(-1, 1),
     main = "Margin Cumulative Distribution Graph",
     xlab = "m",
     ylab = "% Observations",
     col = "blue3",
     lwd = 2)

abline(v = 0,
       col = "red",
       lty = 2,
       lwd = 2)

lines(sort(margins.test),
      1:length(margins.test)/length(margins.test),
      type = "l",
      cex = 0.5,
      col = "green",
      lwd = "2")

legend("topleft",
       c("test", "train"),
       col = c("green", "blue"),
       lty = 1,
       lwd = 2)

evol.test <- errorevol(iris.adaboost,
                       iris.test)
evol.train <- errorevol(iris.adaboost, iris.train)

plot(evol.test$error,
     type = "l",
     ylim = c(0, 1),
     main = "Boosting error vs Number of Trees",
     xlab = "Iterations",
     ylab = "Error",
     col = "red",
     lwd = 2)

lines(evol.train$error,
      cex = .5,
      col = "blue",
      lty = 2,
      lwd = 2)

legend("topleft",
       c("test", "train"),
       col = c("red", "blue"),
       lty = 1:2,
       lwd = 2)
