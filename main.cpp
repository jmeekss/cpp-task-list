/*
* Author:	J Meeks
* Purpose:	This program allows the user to manage a list of tasks
*/

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

string taskFileName = "task_list.txt";

//declares Task class
class Task {
	//private variables of task class
	private:
		string taskName;
		char completed;
	//public functions of Task class
	public:
		//constructor for class
		Task(string newTask, char taskState) {
			set_task(newTask);
			set_completed(taskState);
		}
		
		//sets task for Task object
		void set_task(string newTask) {
			taskName = newTask;
		}

		//sets completed state for Task object
		void set_completed(char state) {
			completed = state;
		}

		//gets the task from the Task object
		string get_task() const {
			return taskName;
		}

		//gets the completed state for the Task object
		char get_completed() const {
			return completed;
		}

		//overloads << operator
		friend ostream& operator<< (ostream& out, const Task& thisTask) {
			out << thisTask.get_task();
			return out;
		}

};

//declare TaskList class
class TaskList {

	//declares vector for list of Task objects
	private:
		vector<Task> tasks;

	//declares public functions of TaskList class
	public:
		//overloads += operator
		TaskList operator+= (const Task& toAdd) {
			tasks.push_back(toAdd);
			return *this;
		}

		//gets the number of total tasks
		int get_number_tasks() const {
			return tasks.size();
		}

		//gets the number of incomplete tasks
		int get_number_not_done() const {
			int count = 0;
			for(unsigned int i = 0; i < tasks.size(); i++){
				if(tasks[i].get_completed() == '0'){//loops through task list and adds to counter if task is not completed
					count++;
				}
			}
			return count;
		}

		//overloads [] operator
		Task& operator[] (int i) {
			return tasks[i];
		}

};

//declares TaskIO class
class TaskIO {
	//declares static functions for TaskIO class
	public:
		//takes file containing tasks and saves them in a TaskList object
		static TaskList write_in_tasks() {
			//creates a TaskList object
			TaskList allTasks;
	
			//opens the file containing tasks, notifies user if there was an error
			ifstream taskFile;
			taskFile.open(taskFileName);
		
			if(!taskFile) {
				cout << "Error opening \"" + taskFileName + "\"" << endl;
				return allTasks;
			}

			//declares variables for creating and filling TaskList object
			string newTask, line;
			char completed;

			//loops through file line by line
			while(getline(taskFile, line)){
				//gets the task and completed state from each line of the file
				istringstream ss(line);
				getline(ss, newTask, '\t');
				ss >> completed;

				//creates a new Task object, and adds it to the list
				Task addTask(newTask, completed);
				allTasks += addTask;
			}
	
			//closes file and returns the TaskList object
			taskFile.close();
			return allTasks;
		}

		//writes the TaskList object to a file
		static void write_out_tasks(TaskList allTasks) {
			//opens the task list file and clears it
			ofstream taskFile;
			taskFile.open(taskFileName, ios::out);

			//loops through list of tasks and adds them to the file
			for(int i = 0; i < allTasks.get_number_tasks(); i++) {
				taskFile << allTasks[i] << "\t" << allTasks[i].get_completed() << '\n';
			}

			//close the file
			taskFile.close();
			return;
		}

};

char askCommand();
int getInt(int max);

int main() {
	//creates a TaskList object from the file containing tasks
	TaskList allTasks = TaskIO::write_in_tasks();

	//prints title and commands to screen
	cout << "Task List\n" 
		<< "\nCOMMANDS"
		<< "\nv - View pending tasks"
		<< "\na - Add a task"
		<< "\nc - Complete a task"
		<< "\nh - History of completed tasks"
		<< "\nx - Exit\n";

	//declares variable for command chosen by user
	char command;

	//loop for user to enter commands
	while(command != 'x'){
		//asks user what command they want
		command = askCommand();

		if(command == 'v') { //if user chooses v command
			//loops through all tasks, and prints the incomplete ones
			int count = 1;
			for(int i = 0; i < allTasks.get_number_tasks(); i++){
				if(allTasks[i].get_completed() == '0'){
					cout << count << ". " << allTasks[i] << '\n';
					count++;
				}
			}

		} else if(command == 'a') { //if user chooses a command
			//asks user for description of new task
			string addNewTask;
			cout << "Description: ";
			getline(cin, addNewTask);
			
			//creates a new Task object, and adds it to the list
			Task newTask(addNewTask, '0');
			allTasks += newTask;

		} else if(command == 'c') { //if user chooses c
			//loops through all the tasks until the task the user chose is found
			int count = 1;
			int num = getInt(allTasks.get_number_not_done());
                        for(int i = 0; i < allTasks.get_number_tasks(); i++){
                                if(allTasks[i].get_completed() == '0'){
					if(count == num){
						allTasks[i].set_completed('1'); //sets the state of the chosen task to 1(done)
					}
                                        count++;
                                }
                        }
		} else if(command == 'h') { //if user chooses h
			//loops through all tasks, and print the complete ones
			int count = 1;
                        for(int i = 0; i < allTasks.get_number_tasks(); i++){
                                if(allTasks[i].get_completed() == '1'){
                                        cout << count << ". " << allTasks[i] << " (DONE!)\n";
                                        count++;
                                }
                        }
		} else if(command == 'x') { //if user chooses x
			//exits the loop
			break;
		} else {
			//prints that user has chosen invalid command
			cout << "Invalid command\n";
		}
		//writes updated task list to file
		TaskIO::write_out_tasks(allTasks);
	}

	//prints ending message and returns 0 to indicate normal program exit
	cout << "Bye!\n";
	return 0;
}

//asks the user if they want to play again
char askCommand(){
        //variables for valid selection and choice user enters
        int validSelection = 0;
        char choice;

        //loops until user enters valid choice
        while(validSelection == 0){
                        //ask user for choice, and clears buffer after they enter it
                        cout << "\nCommand: ";
                        cin >> choice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        //Tells user if their choice is invalid
                        if(tolower(choice) != 'v' && tolower(choice) != 'a' && tolower(choice) != 'c' && tolower(choice) != 'h' && tolower(choice) != 'x'){
                                cout << "Invalid command\n";
                        } else {
                                return tolower(choice);
                        }
                }

        return 'q';
}

//gets valid integer input from the user
int getInt(int max){
        //variables for valid selection and the number chosen by the user
        int validSelection = 0, number;
        //loops until a valid selection is chosen
        while(validSelection == 0){
                        //asks for integer from user, and checks to make sure a number is entered, then clears buffer
                        cout << "Number: ";
                        if(!(cin >> number)){
                                number = -1;
                                cin.clear();
                        }
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        //Tells user if their number is invalid, or returns valid number
                        if(number < 1 || number > max){
                                cout << "Invalid selection\n";
                        } else {
                                return number;
                        }
        }
        return 0;
}

