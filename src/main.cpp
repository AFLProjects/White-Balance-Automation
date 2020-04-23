#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;


void createFile(string path) 
{
   ofstream file(path);
   file.close();
}

void createWriteFile(string path,string text) 
{
   ofstream file(path);
   file << text;
   file.close();
}

char* ReadBytesFile(string path) 
{
    /*Open file*/
    FILE *fp;
    fp= fopen(path.c_str(), "rb");    
    
    /*Get length*/
    fseek(fp, 0, SEEK_END);         
    long length= ftell(fp);     
    rewind(fp);  
              
    /*Transfer data*/
    char *file_data= (char *)malloc((length+1)*sizeof(char));
    fread(file_data, length, 1, fp);
    
    return file_data;  
}

const string baseDirectory= "C:\\Users\\cfluder\\Desktop\\Github\\UVIDEO\\master_repo\\Uvideo\\src";

int main(){

  /*<==FAST IO==>*/
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  /*<==TITLE==>*/
  const char *title =
    "     ----     `---.      .---.       .---.      .---`       `-----..`            `--------.            .://:-`          "
    "    `MMMM`    -MMMm      -NMMM:     -NMMM:      hMMM/       oMMMMMMMMNh/`        :MMMMMMMMd         /hNMMMMMMMms-       "
    "    `MMMM`    -MMMm       :MMMN.   `mMMM+       hMMM/       oMMMmssymMMMN:       :MMMNssss+       .dMMMMmhyhNMMMMy      "
    "    `MMMM`    -MMMm        oMMMd`  hMMMs        hMMM/       oMMMs    +MMMN.      :MMMm`````      `mMMMh.     :mMMMy     "
    "    `MMMM`    -MMMm         yMMMy oMMMh         hMMM/       oMMMs     mMMM+      :MMMMMMMMo      /MMMN        :MMMM`    "
    "    `MMMM`    :MMMm          dMMMyMMMd`         hMMM/       oMMMs    `NMMM:      :MMMNyyyy/      -MMMM-       oMMMm     "
    "     mMMMs`  .hMMMy          `mMMMMMN.          hMMM/       oMMMs  .+mMMMh       :MMMm            sMMMNs:.`./hMMMM:     "
    "     -NMMMMNNMMMMy`           .NMMMN-           hMMM/       oMMMMMMMMMMm+        :MMMMMMMMd        /mMMMMMMMMMMMh-      "
    "      `/sdmNmmho.              :hhh:            shhh:       /hhhhhhhs+-          -hhhhhhhhs          -ohdmNmdy+.        ";
  cout << title << "\n by Adrian Fluder" <<endl;
    
  /*<==Commands==>*/
  const char *commands =
    "\nCommands :               "
    "\n\t-settings              //Shows the settings"
    "\n\t-settings -s           //Set setting manually"
    "\n\t-settings -a <dir>     //Set the settings automatically from videos in <dir>"
    "\n\t-edit <dir>            //Edits the videos/images in <dir>";
  cout << commands << endl;
    
  std::getchar();
  return 0;

}

