#Machine Learning library

This is just a side project. I aim to write from scratch popular machine
learning classifiers, in either C++ or Python. This would hopefully serve
as both practice in writing code and maintaining good practices, as well
as a way to reinforce my understanding of the theory behind the classifier.

If I suceed in making a decent amount of classifiers, I'll try and package
it into a single entity. This is in no way meant to compete with existing
packages or libraries, and is just an amateur attempt.


#####Random Forest Classifier

First one to be written, this works in a simple manner. For a good overview,
refer to [this](http://en.wikipedia.org/wiki/Random_forest) wikipedia link.

It uses a forest, or set of trees to classify data based on a given training
set. In simple words, it works in the following manner:

1. Choose a random variable ID.
2. Generate a random threshold for this variable, based on the maximum and
minimum values of this variable in the data.
3. Partition the data based on whether the required point is greater or
lesser than the given threshold.
4. Follow these steps succesively, building a tree of variables/thresholds.
5. Pass the data down in steps, and stop when you hit criteria defined earlier.
6. Create even more trees in the same manner, and stop when you feel you have
enough.
7. To classify, simply pass the data through the tree, and select the class
which the highest number of trees think this particular datapoint belongs to.


The main drawback with Random Forest classifiers seem to be that they can
tend to learn the training data [too well](http://en.wikipedia.org/wiki/Overfitting)

You can avoid this by limiting the depth of each tree. It has also been proven
that increasing the number of trees you have in your forest tends to increase
the accuracy of your model asymptotically, until it hits a limit.
