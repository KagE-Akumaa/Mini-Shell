#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

void checkSpaces(string &str) {
  string temp = "";
  bool oneSpace = false;
  // For whitespaces from back we can just pop back for front we can find the
  // ith index from where spaces stops -> Works

  int idx = -1;
  for (int i = 0; i < str.size(); i++) {
    if (str[i] != ' ') {
      idx = i;
      break;
    }
  }
  if (idx == -1) {
    // Means no charcter
    return;
  }
  while (str.back() == ' ') {
    str.pop_back();
  }
  for (int i = idx; i < str.size(); i++) {
    if (str[i] != ' ')
      oneSpace = false;
    if (oneSpace)
      continue;
    if (str[i] == ' ')
      oneSpace = true;
    temp.push_back(str[i]);
  }
  str = temp;
}
void getCommandArgs(string &command, vector<string> &arguments,
                    const string &str) {
  // Till first space its the command
  int idx = -1;
  for (int i = 0; i < str.size(); i++) {
    if (str[i] == ' ') {
      idx = i;
      break;
    }
    command.push_back(str[i]);
  }

  if (idx == -1) {
    // Means no space it only contains command no arguments
    return;
  }
  string temp = "";
  for (int i = idx + 1; i < str.size(); i++) {
    if (str[i] == ' ') {
      arguments.push_back(temp);
      temp = "";
      continue;
    }
    temp += str[i];
  }
  if (!temp.empty()) {
    arguments.push_back(temp);
  }
}
int main(int argc, char **argv) {
  while (true) {
    cout << "Mini->  ";

    // read input from the user
    string str;
    getline(cin, str);

    // Need to think how to check enter should we do if (str is empty) or we
    // need to get the asci code of enter -> ans is to check empty as getline
    // removes the new line.
    /*
    if (enter){
      continue;
    }
    */
    if (str.empty())
      continue;

    // Now str will have command and some arguments
    // need to parse this
    // TODO: make multiple space if any as one
    checkSpaces(str);
    // this works but it does not trim whitespaces from the front and back ->
    // now it does work

    // Now we need to split into command and arguments -> Done
    string command = "";
    vector<string> arguments;

    cout << str << endl;
    getCommandArgs(command, arguments, str);

    cout << command << endl;

    for (string &s : arguments)
      cout << s << " ";

    // Now stage 3 we need to use fork to copy the process and that child will
    // become the command and then executes it while the parent waits for the
    // process to complete
    // TODO: need to understand how to use fork -> man pages
    pid_t pid = fork();
    // -1 on error
    if (pid < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    if (pid == 0) {
      // Child will call the execvp and executes the command + arguments
    }
  }
}
