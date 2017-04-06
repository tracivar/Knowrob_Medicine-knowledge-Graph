#include <string>
#include <iostream>
#include "std_msgs/String.h"

#include <ros/ros.h>

//This requires further modification for the environment path settings
#include </home/raw/catkin_ws/src/knowrob/json_prolog/include/json_prolog/prolog.h>

using namespace std;
using namespace json_prolog;


//If the query result is literal, then cut the unnecessary part. 

std::string cutLiteral(std::string s)
{
	size_t pos = 0;
	int num = s.length();
		
	if( (pos = s.find("lang")) != std::string::npos )
		
		return s.substr(17, (num-19) );
	else
		return s.substr(8, (num-9) );
}


// If the query result is IRI, then it needs to be queried again to obtain the useful information.

std::string findIRI(std::string s)
{
	Prolog pl;
	std::string queryKeyIRI = "owl_has('"+ s +"',P,O)";
	PrologQueryProxy bdgsKeyIRI = pl.query(queryKeyIRI.c_str());
	for(PrologQueryProxy::iterator it=bdgsKeyIRI.begin();
      it != bdgsKeyIRI.end(); it++)
  {
    PrologBindings bdgKeyIRI = *it;

    std::string objFirst = bdgKeyIRI["O"].toString();
	if (objFirst[0] == 'l')
		return cutLiteral(objFirst);
	else continue; 
  }
	
	return "hello";
}


//Cut IRI from "#" to obtain necessary information.

std::string cutIRI(std::string s)
{
	size_t pos = s.find("#");

	return s.substr(pos+1);
}


//Print the appropriate result.

std::string PrintResult(std::string s)
{
	std::string delimiterIRI = "http://www.example.com/";
	size_t pos = 0;
	if( (pos = s.find(delimiterIRI)) != std::string::npos )
		return findIRI(s);
	else if( (pos = s.find("#")) != std::string::npos ) 
		return cutIRI(s);
	else
		return cutLiteral(s);
}


//Reciving the callback data and process it.

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  std::string keyWord = msg->data;
 
  //First query the keyword data from the topic.

  Prolog pl;

  std::string queryKey = "owl_has(S, rdfs:label, literal(" + keyWord + "))";

  PrologQueryProxy bdgsKey = pl.query(queryKey.c_str());

  //Obtain the IRI for the keyword in the knowledge graph and query the IRI.

  PrologQueryProxy::iterator it=bdgsKey.begin();
  PrologBindings bdgKey = *it;
  std::string keywordIRI = bdgKey["S"].toString();
  
  std::string queryKeyIRI = "owl_has('"+keywordIRI +"',P,O)";

  PrologQueryProxy bdgsKeyIRI = pl.query(queryKeyIRI.c_str());

  //Print all the information generated from the IRI query result. 

  for(PrologQueryProxy::iterator it=bdgsKeyIRI.begin();
      it != bdgsKeyIRI.end(); it++)
  {
    PrologBindings bdgKeyIRI = *it;

    std::string preFirst = bdgKeyIRI["P"].toString();
    std::string objFirst = bdgKeyIRI["O"].toString();

    cout << PrintResult(preFirst) <<" : ";
    cout << PrintResult(objFirst) << endl; 
    	
  } 

   cout << endl;

}



//Registering a ROS node and subscribe to the topic.

void listener(int argc, char *argv[])
{
  ros::init(argc, argv, "medicine");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("xfspeech", 1000, chatterCallback);
  ros::spin();
  
}



//Main function

int main(int argc, char *argv[])
{
  listener( argc, argv);
  return 0;
}
