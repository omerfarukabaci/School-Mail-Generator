/* @Author
* Student Name: Ömer Faruk ABACI
* Student ID : 150150037
* Date: 01.11.2017
*/

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <ctype.h>
#include <string.h>
#include <fstream>
#include <string>
using namespace std;

struct studentInfo {
	studentInfo *next, *previous;
	char *name;
	char *surname;
	char *email;
};

struct surnameMap {
	surnameMap *next;
	studentInfo *infohead;
	char *surname;
};

struct circularLinkedList {
	surnameMap *head, *tail;
	int surnameCount = 0;
	void createList();
	void insertAll();
	void listAll();
	void insertNewRecord(char* name, char* surname);
	void addInfo(char* name, char* surname, surnameMap* record, bool new_rec);
	void deleteStudent(char* name, char* surname);
	void deleteSurnameNode(char* surname);
	void updateList(char *name_before, char* surname_before, char* name_after, char* surname_after);
	void writeToFile();
	void exitProgram();
};

void circularLinkedList::createList() {
	head = NULL;
	tail = NULL;
	surnameCount = 0;
}

void circularLinkedList::insertAll() {
	ifstream infile;
	string data;
	bool rec_name = true;
	int word_count = 1;
	infile.open("newStudentsList.txt");

	while (getline(infile, data)) {
		char temp_name[30];
		char temp_surname[30];
		for (unsigned int i = 0; i < data.length(); i++) {
			if (data[i] == ' ' || data[i] == '\t') {
				word_count++;
				data[i] = ' ';
			}
		}
		for (unsigned int i = 0, k = 0, j = 0; i < data.length(); i++) {
			if (data[i] == ' ') {
				k++;
				if (k == word_count - 1) {
					temp_name[j] = '\0';
					rec_name = false;
					j = 0;
				}
			}
			if (rec_name) {
				temp_name[j] = data[i];
				j++;
			}
			else if (data[i] != ' ') {
				temp_surname[j] = data[i];
				j++;
			}
			if (i == data.length() - 1) temp_surname[j] = '\0';
		}
		rec_name = true;
		word_count = 1;
		insertNewRecord(temp_name, temp_surname);
	}
	infile.close();
}

void circularLinkedList::listAll() {//OPTIONAL
	surnameMap *traverse;
	studentInfo *traverse_info;
	traverse = head;
	if (traverse) {
		cout << "------------------------------------------------------------" << endl;
		while (traverse != tail) {
			traverse_info = traverse->infohead;
			while (traverse_info) {
				cout << "Name: " << traverse_info->name << endl;
				cout << "Surname: " << traverse_info->surname << endl;
				cout << "E-mail: " << traverse_info->email << endl;
				cout << "------------------------------------------------------------" << endl;
				traverse_info = traverse_info->next;
			}
			traverse = traverse->next;
		}

		traverse_info = traverse->infohead;
		while (traverse_info) {
			cout << "Name: " << traverse_info->name << endl;
			cout << "Surname: " << traverse_info->surname << endl;
			cout << "E-mail: " << traverse_info->email << endl;
			cout << "------------------------------------------------------------" << endl;
			traverse_info = traverse_info->next;
		}
	}
	else cout << endl << "No record found!" << endl << endl;
	
	return;
}

void circularLinkedList::insertNewRecord(char* name , char* surname) {
	surnameMap *record, *traverse;
	traverse = head;

	if (head == NULL) {
		record = new surnameMap;
		record->surname = new char[strlen(surname)];
		strcpy(record->surname, surname);
		head = record;
		tail = record;
		record->next = record;
		surnameCount++;
		cout << "1 Record has been added which is \"" << name << " " << surname << "\". " << endl;
		addInfo(name, surname, record, true);
		return;
	}
	else {
		if (strcmp(surname, head->surname) == -1) {
			record = new surnameMap;
			record->surname = new char[strlen(surname)];
			strcpy(record->surname, surname);
			record->next = head;
			tail->next = record;
			head = record;
			surnameCount++;
			cout << "1 Record has been added which is \"" << name << " " << surname << "\". " << endl;
			addInfo(name, surname, record, true);
			return;
		}
		while (true) {
			if (strcmp(surname, traverse->surname) == 0) {
				cout << "1 Record has been added which is \"" << name << " " << surname << "\". " << endl;
				addInfo(name, surname, traverse, false);
				return;
			}
			else if (strcmp(surname, traverse->surname) == 1 && strcmp(surname, (traverse->next)->surname) == -1) {
				record = new surnameMap;
				record->surname = new char[strlen(surname)];
				strcpy(record->surname, surname);
				record->next = traverse->next;
				traverse->next = record;
				surnameCount++;
				cout << "1 Record has been added which is \"" << name << " " << surname << "\". " << endl;
				addInfo(name, surname, record, true);
				return;
			}
			traverse = traverse->next;
			if (traverse == tail) break;
		}

		if (strcmp(surname, traverse->surname) == 0) {
			cout << "1 Record has been added which is \"" << name << " " << surname << "\". " << endl;
			addInfo(name, surname, traverse, false);
			return;
		}

		record = new surnameMap;
		record->surname = new char[strlen(surname)];
		strcpy(record->surname, surname);
		tail->next = record;
		record->next = head;
		tail = record;
		surnameCount++;
		cout << "1 Record has been added which is \"" << name << " " << surname << "\". " << endl;
		addInfo(name, surname, record, true);
		return;
	}
}

void circularLinkedList::addInfo(char* name, char* surname, surnameMap* record, bool new_rec) {
	studentInfo *info_record, *info_traverse;
	char temp_email[50];
	int name_count = 1;
	info_record = new studentInfo;
	info_record->name = new char[strlen(name)];
	strcpy(info_record->name, name);
	info_record->surname = new char[strlen(surname)];
	strcpy(info_record->surname, surname);
	if (new_rec) {
		record->infohead = info_record;
		info_record->next = NULL;
		info_record->previous = NULL;
		for (unsigned int i = 0; i < strlen(surname); i++) {
			temp_email[i] = tolower(surname[i]);
		}
		temp_email[strlen(surname)] = '\0';
		strcat(temp_email, "@itu.edu.tr");
		info_record->email = new char[strlen(temp_email)];
		strcpy(info_record->email, temp_email);
		cout << "Program assigns " << info_record->email << " address for the given request." << endl << endl;
		return;
	}
	else {
		info_traverse = record->infohead;
		for (unsigned int i = 0; i < strlen(surname); i++) {
			temp_email[i] = tolower(surname[i]);
		}
		temp_email[strlen(surname)] = '\0';

		if (strcmp(info_traverse->name, name) == 0) {
			info_traverse = record->infohead;
			while (info_traverse) {
				if (strcmp(info_traverse->name, name) == 0) name_count++;
				if (info_traverse->next == NULL) break;
				info_traverse = info_traverse->next;
			}
			if (name_count < 10) {
				char aChar[2];
				aChar[0] = '0' + name_count;
				aChar[1] = '\0';
				strcat(temp_email, aChar);
				strcat(temp_email, "@itu.edu.tr");
			}
			else {
				cout << "SIZE_ERROR: There are more than 9 records with same name and surname." << endl;
				delete info_record;
				return;
			}
			info_record->email = new char[strlen(temp_email)];
			strcpy(info_record->email, temp_email);
			info_traverse = record->infohead;
			info_traverse->previous = info_record;
			info_record->next = info_traverse;
			record->infohead = info_record;
			cout << "Program assigns " << info_record->email << " address for the given request." << endl << endl;
			return;
		}
		else {
			info_traverse = record->infohead;
			int k = 0;
			char toadd[2];
			while (info_traverse) {
				if (strncmp(info_traverse->email, temp_email, strlen(temp_email)) == 0) {
					toadd[0] = tolower(name[k]);
					toadd[1] = '\0';
					strcat(temp_email, toadd);
					k++;
					if (k > strlen(name)) {
						cout << "SIZE_ERROR: Too many people with similar name. Add your nickname and try again." << endl;
						delete info_record;
						return;
					}
					info_traverse = record->infohead;
					continue;
				}
				if (info_traverse->next == NULL) break;
				info_traverse = info_traverse->next;
			}
			strcat(temp_email, "@itu.edu.tr");
			info_record->email = new char[strlen(temp_email)];
			strcpy(info_record->email, temp_email);
			info_traverse = record->infohead;
			info_traverse->previous = info_record;
			info_record->next = info_traverse;
			record->infohead = info_record;
			cout << "Program assigns " << info_record->email << " address for the given request." << endl << endl;
			return;
		}
	}
}

void circularLinkedList::updateList(char *name_before, char* surname_before, char* name_after, char* surname_after) {
	deleteStudent(name_before, surname_before);
	insertNewRecord(name_after, surname_after);
}

void circularLinkedList::deleteStudent(char* name, char* surname) {
	surnameMap *traverse, *traverse_tail;
	traverse = head;
	traverse_tail = tail;
	studentInfo *info_traverse;

	while (true) {
		if (strcmp(traverse->surname, surname) == 0) {
			info_traverse = traverse->infohead;
			break;
		}
		if (traverse == tail) {
			cout << "No records matched with given surname! Please check your spelling. (It is case sensitive.)" << endl << endl;
			return;
		}
		traverse = traverse->next;
		traverse_tail = traverse_tail->next;
	}

	if (strcmp(info_traverse->name, name) == 0) {
		traverse->infohead = info_traverse->next;
		if (traverse->infohead == NULL) {
			delete info_traverse;
			cout << "1 Student Record has been deleted!" << endl << endl;
			if (traverse == head) {
				traverse_tail->next = traverse->next;
				head = traverse->next;
				delete traverse;
				surnameCount--;
				cout << "1 Surname Record has been deleted!" << endl << endl;
				if (surnameCount == 0) {
					head = NULL;
					tail = NULL;
				}
				return;
			}
			else if (traverse == tail) {
				traverse_tail->next = traverse->next;
				tail = traverse_tail;
				delete traverse;
				surnameCount--;
				cout << "1 Surname Record has been deleted!" << endl << endl;
				if (surnameCount == 0) {
					head = NULL;
					tail = NULL;
				}
				return;
			}
			else {
				traverse_tail->next = traverse->next;
				delete traverse;
				surnameCount--;
				cout << "1 Surname Record has been deleted!" << endl << endl;
				if (surnameCount == 0) {
					head = NULL;
					tail = NULL;
				}
				return;
			}
			return;
		}
		if(traverse->infohead) traverse->infohead->previous = NULL;
		delete info_traverse;
		cout << "1 Student Record has been deleted!" << endl << endl;
		return;
	}

	info_traverse = info_traverse->next;

	while (true) {
		if (strcmp(info_traverse->name, name) == 0) {
			if (info_traverse->next == NULL) {
				info_traverse->previous->next = NULL;
				delete info_traverse;
				cout << "1 Student Record has been deleted!" << endl << endl;
				return;
			}
			else {
				info_traverse->previous->next = info_traverse->next;
				info_traverse->next->previous = info_traverse->previous;
				delete info_traverse;
				cout << "1 Student Record has been deleted!" << endl << endl;
				return;
			}
		}
		if (info_traverse->next == NULL) {
			cout << "No records matched with given name! Please check your spelling. (It is case sensitive.)" << endl << endl;
			return;
		}
		info_traverse = info_traverse->next;
	}

}

void circularLinkedList::deleteSurnameNode(char* surname) {
	surnameMap *traverse;
	studentInfo *info_traverse;
	int info_count = 0;
	traverse = head;
	while (true) {
		if (strcmp(traverse->surname, surname) == 0) {
			info_traverse = traverse->infohead;
			while (true) {
				info_count++;
				if (info_traverse->next == NULL) break;
				info_traverse = info_traverse->next;
			}
			for (int i = 0; i < info_count; i++) {
				info_traverse = traverse->infohead;
				deleteStudent(info_traverse->name, info_traverse->surname);
			}
			return;
		}
		if (traverse == tail) break;
		traverse = traverse->next;
	}
}

void circularLinkedList::writeToFile() {
	if (head) {
		ofstream outfile;
		outfile.open("emailList.txt");
		surnameMap *traverse;
		studentInfo *traverse_info;
		traverse = head;
		while (traverse != tail) {
			traverse_info = traverse->infohead;
			while (traverse_info) {
				outfile << "Name: " << traverse_info->name << endl;
				outfile << "Surname: " << traverse_info->surname << endl;
				outfile << "E-mail: " << traverse_info->email << endl << endl;
				traverse_info = traverse_info->next;
			}
			traverse = traverse->next;
		}

		traverse_info = traverse->infohead;

		while (traverse_info) {
			outfile << "Name: " << traverse_info->name << endl;
			outfile << "Surname: " << traverse_info->surname << endl;
			outfile << "E-mail: " << traverse_info->email << endl << endl;
			traverse_info = traverse_info->next;
		}
		cout << "\"emailList.txt\" file has been created." << endl << endl;
		outfile.close();
	}
	else cout << endl << "No record found!" << endl << endl;
	return;

}

void circularLinkedList::exitProgram() {
	surnameMap *traverse;
	traverse = head;
	while (traverse != tail) {
		deleteSurnameNode(traverse->surname);
		traverse = traverse->next;
	}
	deleteSurnameNode(traverse->surname);
	return;
}

int printMenu();

circularLinkedList surnameList;

int main() {
	int choice;
	char name[30], surname[30], name_before[30], surname_before[30];
	choice = printMenu();
	while (choice != 8) {
		switch (choice) {
			case 1:
				surnameList.createList();
				cout << endl << "List has been created." << endl << endl;
				break;
			case 2:
				surnameList.insertAll();
				break;
			case 3:
				cout << "Please enter the name of the student you want to add (Case Sensitive): ";
				cin.ignore(1000, '\n');
				cin.getline(name, 30);
				cout << endl;
				cout << "Please enter the surname of the student you want to add (Case Sensitive): ";
				cin.getline(surname, 30);
				cout << endl;
				surnameList.insertNewRecord(name, surname);
				break;
			case 4:
				cout << "Please enter the name of the student you want to delete (Case Sensitive): ";
				cin.ignore(1000, '\n');
				cin.getline(name, 30);
				cout << endl;
				cout << "Please enter the surname of the student you want to delete (Case Sensitive): ";
				cin.getline(surname, 30);
				cout << endl;
				surnameList.deleteStudent(name, surname);
				break;
			case 5:
				cout << "Please enter the surname you want to delete (Case Sensitive): ";
				cin.ignore(1000, '\n');
				cin.getline(surname, 30);
				cout << endl;
				surnameList.deleteSurnameNode(surname);
				break;
			case 6:
				cout << "Please enter the name of the student you want to change (Case Sensitive): ";
				cin.ignore(1000, '\n');
				cin.getline(name_before, 30);
				cout << endl;
				cout << "Please enter the surname of the student you want to change (Case Sensitive): ";
				cin.getline(surname_before, 30);
				cout << endl;
				cout << "Please enter the new name of the student (Case Sensitive): ";
				cin.getline(name, 30);
				cout << endl;
				cout << "Please enter the new surname of the student (Case Sensitive): ";
				cin.getline(surname, 30);
				cout << endl;
				surnameList.updateList(name_before, surname_before, name, surname);
				break;
			case 7:
				surnameList.writeToFile();
				break;
			case 8:
				surnameList.exitProgram();
				break;
			default:
				cout << "Wrong input, try again." << endl;
		}
		choice = printMenu();
	}
	return 0;
}

int printMenu() {
	int choice;
	cout << "e-mail Address Program:" << endl
		<< "1) Create" << endl
		<< "2) Insert All" << endl
		<< "3) Insert New Record" << endl
		<< "4) Delete Student" << endl
		<< "5) Delete Surname Node" << endl
		<< "6) Update" << endl
		<< "7) Write to File" << endl
		<< "8) Exit" << endl << endl
		<< "Enter a choice: ";
	cin >> choice;
	return choice;
}
