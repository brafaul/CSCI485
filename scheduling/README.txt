Name: README.txt
Project: Assignment 6 CSCI 485
Authors: Brayden Faulkner and Hayden Nanny

	This project creates four programs to run four different scheduling
algorthims to run over a given set of task. You can choose which algorithim
to run by typing make then the abbrevation of scheduling alogrithim. Then 
run the program by typing ./ then the abbrevation followed by the filename 
of the file containing the schedule  Each of the programs is made of two 
functions, add and schedule. 
	The add function is fairly standard across all the programs. It
takes the information about the task to add to the list as parameters, then
uses those parameters to make pointer to a task called temp, that is then
added to the list. This function also increment the list index variable, 
so the file has an accurate representation of what is in the list.
	The schedule function varies from program to program. For the fcfs 
program it first goes through the list and  
