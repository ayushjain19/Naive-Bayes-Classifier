# Naive-Bayes-Classifier
Naive Bayes Classifier with stop words | Naive Bayes Classifier without stop words | Binary Naive Bayes Classifier

**Algorithm:**
Learn_Naive_Bayes_Text(*Examples*, V):

*Examples is a set of text documents along with their target values. V is the set of all possible target values. This function learns the probability terms P(wk | v,), describing the probability that a randomly drawn word from a document in class vj will be the English word wk. It also learns the class prior probabilities P(vj).*

1. *collect all words, punctwtion, and other tokens that occur in Examples*
    * *Vocabulary* = the set of all distinct words and other tokens occurring in any text document
from *Examples*
1. *calculate the required P(vj) and P(wk | vj) probability terms*
    * For each target value vj in V do
      * docsj = the subset of documents from *Examples* for which the target value is vj
      * P(vj) = *|docsj|* / *|Examples|*
      * *Textj* = a single document created by concatenating all members of *docsj*
      * n = total number of distinct word positions in *Textj*
      * for each word *wk* in Vocabulary:
          * *nk* = number of times word *wk* occurs in *Textj*
          * *P(wk | vj) =  (nk + 1) / (n + |Vocabulary|)*

Classify_Naive_Bayes_Text(Doc):

*Return the estimated target value for the document Doc. ai denotes the word found in the ith position within Doc.*
    
  * *positions* = all word positions in *Doc* that contain tokens found in *Vocabulary*
  * Return VNB, where VNB = argmax [ P(vj) x Product over all i (P(ai | vj)) ]

****
**Dataset information:**
* The core dataset contains 50,000 reviews split evenly into 25k train and 25k test sets. The overall distribution of labels is balanced (25k pos and 25k neg).

* In the entire collection, no more than 30 reviews are allowed for any given movie because reviews for the same movie tend to have correlated ratings.

* A negative review has a score <= 4 out of 10, and a positive review has a score >= 7 out of 10. Thus reviews with more neutral ratings are not included in the train/test sets.

* The first number in each review is the score.

* A line with 0:7 in a .txt file means the first word in [imdb.txt] (the) appears 7 times in that review.

****

**Results:**

*Naive Baiyes without removing stop words*
* Accuracy: 0.821
* Precision: 0.797332
* Recall: 0.8608
* F-measure: 0.827852

*Naive Baiyes with stop words removed*
* Accuracy: 0.8142
* Precision: 0.859826
* Recall: 0.7508
* F-measure: 0.801623

*Boolean Naive Baiyes with negative word meaning handled accordingly*
* Accuracy: 0.821
* Precision: 0.816618
* Recall: 0.82792
* F-measure: 0.82223
