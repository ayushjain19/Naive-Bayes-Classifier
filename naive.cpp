#include <bits/stdc++.h>

#define mp make_pair
#define pb push_back
#define ff first
#define ss second
#define ll long long
#define MAX 505
#define mod 1000000007
#define INF 1000007

using namespace std;


int main()
{
	string str;
	int i, j, k;
	vector<int> vocabulary(89528, 0);
	map<int, int> positive;
	map<int, int> negative;
	map<string, int> word_map;
	int confusion_matrix[2][2];
	confusion_matrix[0][0] = confusion_matrix[0][1] = confusion_matrix[1][0] = confusion_matrix[1][1] = 0;
	int n_positive = 0, n_negative = 0, wk, nk;


	// Take input from file and fill in the vocabulary, positive, negative
	ifstream infile("training_data.txt");
	// Reading file
	while (infile)
	{
		string s;
	    if (!getline( infile, s )) break;

	    istringstream ss( s );
	    vector <string> record;

	    while (ss)
	    {
	      string s;
	      if (!getline( ss, s, ' ' )) break;
	      record.push_back( s );
	    }
	    int num;
	    stringstream temp(record[0]);
	    temp >> num;
	    for(i = 1; i < record.size(); i++)
	    {
	    	int a, b;
	    	string a_str = "", b_str = "";
	    	for(j = 0; record[i][j] != ':'; j++)
	    		a_str += record[i][j];
	    	for(k = j+1; k < record[i].length(); k++)
	    		b_str += record[i][k];
	    	stringstream temp1(a_str);
	    	temp1 >> a;
	    	stringstream temp2(b_str);
	    	temp2 >> b;
	    	vocabulary[a] = 1;
	    	if(num >= 7)
	    	{
	    		n_positive += b;
	    		if(positive.find(a) == positive.end())
	    			positive[a] = b;
	    		else positive[a] += b;
	    	}
	    	else
	    	{
	    		n_negative += b;
	    		if(negative.find(a) == negative.end())
	    			negative[a] = b;
	    		else negative[a] += b;
	    	}
	    }
	}
	infile.close();;


	// Form the word map -> {word: index} This will be used for getting index of stop words
	// positive and negative are the dictionaries pointing to number of times that particular word occurs
	// n_positive has total number of distinct word positions in positive examples (similar for n_negative)

	cout << "Predicting" << endl;
	// Declaring Probability array
	float P[89528][2];
	for(i = 0; i < 89528; i++)
	{
		P[i][0] = 0;
		P[i][1] = 0;
	}
	int vocabulary_size = 0;

	// Training the classifier
	for(i = 0; i < vocabulary.size(); i++)
	{
		if(vocabulary[i] == 1)
		{
			vocabulary_size += 1;
			nk = positive[i];
			P[i][0] = float(nk + 1) / (n_positive + vocabulary_size);
		}
	}

	for(i = 0; i < vocabulary.size(); i++)
	{
		if(vocabulary[i] == 1)
		{	
			nk = negative[i];
			P[i][1] = float(nk + 1) / (n_negative + vocabulary_size);
		}
	}

	float positive_prob, negative_prob, accuracy, precision, recall, f_measure;
	int prediction, classification;


	// Testing file input
	ifstream testingfile("testing_data.txt");
	// Reading file
	while (testingfile)
	{
	// For every doc
	// doc is the dictionaries pointing to number of times that particular word occurs in the current document
		
		string s;
	    if (!getline( testingfile, s )) break;

	    istringstream ss( s );
	    vector <string> record;
	    while (ss)
	    {
	      string s;
	      if (!getline( ss, s, ' ' )) break;
	      record.push_back( s );
	    }
	    int num;
	    stringstream temp(record[0]);
	    temp >> num;

	    positive_prob = log2(0.5), negative_prob = log2(0.5);
		// For every data in doc
		for(i = 1; i < record.size(); i++)
	    {
			vector<int>data(2,0);
	    	string a_str = "", b_str = "";
	    	for(j = 0; record[i][j] != ':'; j++)
	    		a_str += record[i][j];
	    	for(k = j+1; k < record[i].length(); k++)
	    		b_str += record[i][k];
	    	stringstream temp1(a_str);
	    	temp1 >> data[0];
	    	stringstream temp2(b_str);
	    	temp2 >> data[1];

			// If data[0] in vocabulary
			if(vocabulary[data[0]] == 1)
			{
				positive_prob += data[1] * log2(P[data[0]][0]);
				negative_prob +=  data[1] * log2(P[data[0]][1]);
			}
		}
		// cout << positive_prob << " " << negative_prob << endl;
		if(positive_prob > negative_prob)
			prediction = 1;
		else prediction = -1;
		
		// Define actual classification for the current doc if(>=7) then 1. if(<=4) the -1
		if(num >= 7) classification = 1;
		else classification = -1;

		if(classification == 1 && prediction == 1) confusion_matrix[0][0]++;
		else if(classification == 1 && prediction == -1) confusion_matrix[0][1]++;
		else if(classification == -1 && prediction == 1) confusion_matrix[1][0]++;
		else if(classification == -1 && prediction == -1) confusion_matrix[1][1]++;
	}
	testingfile.close();
	cout << confusion_matrix[0][0]  << " " << confusion_matrix[0][1] << endl;
	cout << confusion_matrix[1][0]  << " " << confusion_matrix[1][1] << endl;
	accuracy = float(confusion_matrix[0][0] + confusion_matrix[1][1]) / (confusion_matrix[0][0] + confusion_matrix[0][1] + confusion_matrix[1][0] + confusion_matrix[1][1]);
	precision = float(confusion_matrix[0][0]) / (confusion_matrix[0][0] + confusion_matrix[1][0]);
	recall = float(confusion_matrix[0][0]) / (confusion_matrix[0][0] + confusion_matrix[0][1]);
	f_measure = 2 * (recall * precision) / float(recall + precision);

	// Print all the parameters
	cout << "Naive Bayes without removing stop words!" << endl;
	cout << "Accuracy: " << accuracy << endl;
	cout << "Precision: " << precision << endl;
	cout << "Recall: " << recall << endl;
	cout << "F Measure: " << f_measure << endl;

	






	// Take stop words as input from file and store the mapping index of that stop word in stop_words vector
	ifstream fin;
	fin.open("imdb.txt");
	i = 0;
	while(getline(fin, str))
	{
		word_map[str] = i;
		i++;
	}
	fin.close();

	vector<int> stop_words;
	fin.open("stop_words.txt");
	while(getline(fin, str))
	{
		stop_words.push_back(word_map[str]);
	}
	fin.close();




	// n_positive has total number of distinct word positions in positive examples (similar for n_negative) Note that n_positive and n_negative
	// should not include the position count of the stop words

	// For this, iterate positive/negative dictionary:
	// 				if word not in stop_words vector: then add to n_positive/n_negative

	for(i = 0; i < stop_words.size(); i++)
	{
		n_positive -= positive[stop_words[i]];
		n_negative -= negative[stop_words[i]];
		positive[stop_words[i]] = 0;
		negative[stop_words[i]] = 0;
		if(vocabulary[stop_words[i]] == 1)
		{
			vocabulary[stop_words[i]] = 0;
			vocabulary_size--;
		}
	}

	for(i = 0; i < 89528; i++)
	{
		P[i][0] = 0;
		P[i][1] = 0;
	}

	// Training
	for(i = 0; i < vocabulary.size(); i++)
	{
		if(vocabulary[i] == 1)
		{
			nk = positive[i];
			P[i][0] = float(nk + 1) / (n_positive + vocabulary_size);
		}
	}
	for(i = 0; i < vocabulary.size(); i++)
	{
		if(vocabulary[i] == 1)
		{
			nk = negative[i];
			P[i][1] = float(nk + 1) / (n_negative + vocabulary_size);
		}
	}




	// Testing
	confusion_matrix[0][0] = confusion_matrix[0][1] = confusion_matrix[1][0] = confusion_matrix[1][1] = 0;

	positive_prob = log2(0.5), negative_prob = log2(0.5);

	ifstream testing("testing_data.txt");
	// Reading file
	while (testing)
	{
	// For every doc
	// doc is the dictionaries pointing to number of times that particular word occurs in the current document
		
		string s;
	    if (!getline( testing, s )) break;

	    istringstream ss( s );
	    vector <string> record;
	    while (ss)
	    {
	      string s;
	      if (!getline( ss, s, ' ' )) break;
	      record.push_back( s );
	    }
	    int num;
	    stringstream temp(record[0]);
	    temp >> num;

	    positive_prob = log2(0.5), negative_prob = log2(0.5);

		// For every data in doc
		for(i = 1; i < record.size(); i++)
	    {
			vector<int>data(2,0);
	    	string a_str = "", b_str = "";
	    	for(j = 0; record[i][j] != ':'; j++)
	    		a_str += record[i][j];
	    	for(k = j+1; k < record[i].length(); k++)
	    		b_str += record[i][k];
	    	stringstream temp1(a_str);
	    	temp1 >> data[0];
	    	stringstream temp2(b_str);
	    	temp2 >> data[1];

			// If data[0] in vocabulary
			if(vocabulary[data[0]] == 1)
			{
				positive_prob += data[1] * log2(P[data[0]][0]);
				negative_prob +=  data[1] * log2(P[data[0]][1]);
				// cout << positive_prob << " " << negative_prob << endl;
			}
		}
		if(positive_prob > negative_prob)
			prediction = 1;
		else prediction = -1;
		// Define actual classification for the current doc if(>=7) then 1. if(<=4) the -1
		if(num >= 7) classification = 1;
		else classification = -1;

		if(classification == 1 && prediction == 1) confusion_matrix[0][0]++;
		else if(classification == 1 && prediction == -1) confusion_matrix[0][1]++;
		else if(classification == -1 && prediction == 1) confusion_matrix[1][0]++;
		else if(classification == -1 && prediction == -1) confusion_matrix[1][1]++;
	}
	testing.close();
	cout << confusion_matrix[0][0]  << " " << confusion_matrix[0][1] << endl;
	cout << confusion_matrix[1][0]  << " " << confusion_matrix[1][1] << endl;
	accuracy = float(confusion_matrix[0][0] + confusion_matrix[1][1]) / (confusion_matrix[0][0] + confusion_matrix[0][1] + confusion_matrix[1][0] + confusion_matrix[1][1]);
	precision = float(confusion_matrix[0][0]) / (confusion_matrix[0][0] + confusion_matrix[1][0]);
	recall = float(confusion_matrix[0][0]) / (confusion_matrix[0][0] + confusion_matrix[0][1]);
	f_measure = 2 * (recall * precision) / float(recall + precision);

	// Print all the parameters
	cout << "Naive Bayes after removing stop words!" << endl;
	cout << "Accuracy: " << accuracy << endl;
	cout << "Precision: " << precision << endl;
	cout << "Recall: " << recall << endl;
	cout << "F Measure: " << f_measure << endl;







	// Binary Naive Bayes Classifier

	confusion_matrix[0][0] = confusion_matrix[0][1] = confusion_matrix[1][0] = confusion_matrix[1][1] = 0;

	// Reading the training dataset
	ifstream infilebinary("training_data.txt");
	// Reading file
	while (infilebinary)
	{
		string s;
	    if (!getline( infilebinary, s )) break;

	    istringstream ss( s );
	    vector <string> record;

	    while (ss)
	    {
	      string s;
	      if (!getline( ss, s, ' ' )) break;
	      record.push_back( s );
	    }
	    int num;
	    stringstream temp(record[0]);
	    temp >> num;
	    for(i = 1; i < record.size(); i++)
	    {
	    	int a, b;
	    	string a_str = "", b_str = "";
	    	for(j = 0; record[i][j] != ':'; j++)
	    		a_str += record[i][j];
	    	for(k = j+1; k < record[i].length(); k++)
	    		b_str += record[i][k];
	    	stringstream temp1(a_str);
	    	temp1 >> a;
	    	stringstream temp2(b_str);
	    	temp2 >> b;
	    	vocabulary[a] = 1;
	    	if(num >= 7)
	    	{
	    		n_positive += 1;
	    		if(positive.find(a) == positive.end())
	    			positive[a] = 1;
	    		else positive[a] += 1;
	    	}
	    	else
	    	{
	    		n_negative += 1;
	    		if(negative.find(a) == negative.end())
	    			negative[a] = 1;
	    		else negative[a] += 1;
	    	}
	    }
	}
	infilebinary.close();;


	for(i = 0; i < 89528; i++)
	{
		P[i][0] = 0;
		P[i][1] = 0;
	}
	vocabulary_size = 0;

	// Training
	for(i = 0; i < vocabulary.size(); i++)
	{
		if(vocabulary[i] == 1)
		{
			vocabulary_size += 1;
			nk = positive[i];
			P[i][0] = float(nk + 1) / (n_positive + vocabulary_size);
		}
	}

	for(i = 0; i < vocabulary.size(); i++)
	{
		if(vocabulary[i] == 1)
		{	
			nk = negative[i];
			P[i][1] = float(nk + 1) / (n_negative + vocabulary_size);
		}
	}

	confusion_matrix[0][0] = confusion_matrix[0][1] = confusion_matrix[1][0] = confusion_matrix[1][1] = 0;
	// Reading the testing dataset
	ifstream testingfilebinary("testing_data.txt");
	// Reading file
	while (testingfilebinary)
	{
	// For every doc
	// doc is the dictionaries pointing to number of times that particular word occurs in the current document
		
		string s;
	    if (!getline( testingfilebinary, s )) break;

	    istringstream ss( s );
	    vector <string> record;
	    while (ss)
	    {
	      string s;
	      if (!getline( ss, s, ' ' )) break;
	      record.push_back( s );
	    }
	    int num;
	    stringstream temp(record[0]);
	    temp >> num;

	    positive_prob = log2(0.5), negative_prob = log2(0.5);
		// For every data in doc
		for(i = 1; i < record.size(); i++)
	    {
			vector<int>data(2,0);
	    	string a_str = "", b_str = "";
	    	for(j = 0; record[i][j] != ':'; j++)
	    		a_str += record[i][j];
	    	for(k = j+1; k < record[i].length(); k++)
	    		b_str += record[i][k];
	    	stringstream temp1(a_str);
	    	temp1 >> data[0];
	    	stringstream temp2(b_str);
	    	temp2 >> data[1];

			// If data[0] in vocabulary
			if(vocabulary[data[0]] == 1)
			{
				positive_prob += data[1] * log2(P[data[0]][0]);
				negative_prob +=  data[1] * log2(P[data[0]][1]);
			}
		}
		// cout << positive_prob << " " << negative_prob << endl;
		if(positive_prob > negative_prob)
			prediction = 1;
		else prediction = -1;
		
		// Define actual classification for the current doc if(>=7) then 1. if(<=4) the -1
		if(num >= 7) classification = 1;
		else classification = -1;

		if(classification == 1 && prediction == 1) confusion_matrix[0][0]++;
		else if(classification == 1 && prediction == -1) confusion_matrix[0][1]++;
		else if(classification == -1 && prediction == 1) confusion_matrix[1][0]++;
		else if(classification == -1 && prediction == -1) confusion_matrix[1][1]++;
	}
	testingfilebinary.close();
	cout << confusion_matrix[0][0]  << " " << confusion_matrix[0][1] << endl;
	cout << confusion_matrix[1][0]  << " " << confusion_matrix[1][1] << endl;
	accuracy = float(confusion_matrix[0][0] + confusion_matrix[1][1]) / (confusion_matrix[0][0] + confusion_matrix[0][1] + confusion_matrix[1][0] + confusion_matrix[1][1]);
	precision = float(confusion_matrix[0][0]) / (confusion_matrix[0][0] + confusion_matrix[1][0]);
	recall = float(confusion_matrix[0][0]) / (confusion_matrix[0][0] + confusion_matrix[0][1]);
	f_measure = 2 * (recall * precision) / float(recall + precision);

	// Print all the parameters
	cout << "Binary Naive Bayes!" << endl;
	cout << "Accuracy: " << accuracy << endl;
	cout << "Precision: " << precision << endl;
	cout << "Recall: " << recall << endl;
	cout << "F Measure: " << f_measure << endl;

	return 0;
}