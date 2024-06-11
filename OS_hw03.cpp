// 方法5的結果錯誤
// 已知data list裡會有counter沒有+1的錯誤
//當all_data_i = 7 or 8 時， 1 和 2 的counter沒有從2 + 到 3
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

struct  Data {
    string id;
    int used_counter = 0;
    bool page_fault = false;
};

struct Page {
    string id;
    int used_counter = 0;
};


class Page_Replacement {
    private:
        vector<Data> data_list;
        vector<Page> page_list;
        vector<string> page_history; // page list for all the time slice
    public:
        void CounterUpdate(Data new_counter) { // update counter for LFU and MFU
            for(int i = 0; i < data_list.size(); i++) {
                if(new_counter.id == data_list[i].id) {
                    data_list[i].used_counter = new_counter.used_counter;
                }
            }
        }

        void Infile(string filename) {
            // get the file
            Data temp;
            string skipping;
            ifstream file;
            file.open(filename.c_str());

            if ( file.fail() ) {
                return;
            }

            if (file.is_open()) {
                getline(file, skipping);
                char ch;
                while (file.get(ch)) {
                    if (ch != '\n') {
                        temp.id = string(1, ch);
                        data_list.push_back(temp);
                    }
                }
                
            }
            file.close();
        };
    // testing
        void Print() {
            for(int i = 0; i < data_list.size(); i++) {
                cout << data_list[i].id;
            }

            cout << endl;
        };

        void FIFO(int page_frame) {
            // to do:
            // 1.get the data list
            // 2. process through the list
            // 3. check:
            // if in page frame {
            // no page fault
            // no pop from page list
            // }
            // if not in page frame {
            // page fault
            // pop the first in one
            // push the current one
            // }
            vector<Data> all_data = data_list; // all data
            Data data_temp; // data for push back
            Page page_temp; // page for push back

            for(int all_data_i = 0; all_data_i < all_data.size(); all_data_i++ ) {
                if (page_list.size() < page_frame) {
                    // the page is not full
                    page_temp.id = all_data[all_data_i].id;
                    page_list.insert(page_list.begin(), page_temp);
                }
                
                else {
                    // the page is full
                    bool page_fault = true;

                    // check if it's in the page frame or not
                    for(int check_same = 0; check_same < page_frame; check_same++) {
                        if (all_data[all_data_i].id == page_list[check_same].id) {
                            // already in the list
                            page_fault = false;
                        }
                    }

                    if (page_fault) {
                        page_list.pop_back();
                        data_temp = data_list[all_data_i];
                        page_temp.id = all_data[all_data_i].id;
                        page_list.insert(page_list.begin(),page_temp);
                    }


                }

                // put the full page list into all page list
                string page_temp;
                for (int j = 0; j < page_list.size(); j++) {
                    page_temp = page_temp + page_list[j].id;
                }
                page_history.push_back(page_temp);

            }
        }

        void LRU(int page_frame) {
            // to do:
            // 1.get the data list
            // 2. process through the list
            // 3. check:
            // if in page frame {
            // no page fault
            // pop from page list
            // re push to the list front
            // }
            // if not in page frame {
            // page fault
            // pop the first in one
            // push the current one
            // }
            vector<Data> all_data = data_list; // all data
            Data data_temp; // data for push back
            Page page_temp; // page for push back

            for(int all_data_i = 0; all_data_i < all_data.size(); all_data_i++ ) {
                if (page_list.size() < page_frame) {
                    // the page is not full
                    page_temp.id = all_data[all_data_i].id;
                    page_list.insert(page_list.begin(), page_temp);
                }
                
                else {
                    // the page is full
                    bool page_fault = true;

                    // check if it's in the page frame or not
                    for(int check_same = 0; check_same < page_frame; check_same++) {
                        if (all_data[all_data_i].id == page_list[check_same].id) {
                            // already in the list
                            page_fault = false;
                            break;
                        }
                    }

                    if (page_fault) {
                        page_list.pop_back();
                        data_temp = data_list[all_data_i];
                        page_temp.id = all_data[all_data_i].id;
                        page_list.insert(page_list.begin(),page_temp);
                    }
                    else if (!page_fault) {
                        data_temp = data_list[all_data_i];
                        page_temp.id = all_data[all_data_i].id;
                        page_list.insert(page_list.begin(),page_temp);
                        page_list.pop_back();
                    }
                }

                // put the full page list into all page list
                string page_temp;
                for (int j = 0; j < page_list.size(); j++) {
                    page_temp = page_temp + page_list[j].id;
                }
                page_history.push_back(page_temp);

            }
        }

        void LFU(int page_frame) {
            // to do:
            // 1.get the data list
            // 2. process through the list
            // 3. check:
            // if in page frame {
            // no page fault
            // used counter + 1
            // }
            // if not in page frame {
            // page fault
            // pop the smallest counter one
            // push the current one
            // }
            vector<Data> all_data = data_list; // all data
            Data data_temp; // data for push back
            Page page_temp; // page for push back

            for(int all_data_i = 0; all_data_i < all_data.size(); all_data_i++ ) {
                if (page_list.size() < page_frame) {
                    // the page is not full
                    page_temp.id = all_data[all_data_i].id;
                    page_temp.used_counter = 1;
                    data_list[all_data_i].used_counter++;
                    page_list.insert(page_list.begin(), page_temp);
                }
                
                else {
                    // the page is full
                    bool page_fault = true;

                    // check if it's in the page frame or not
                    int page_i = 0;
                    for(int check_same = 0; check_same < page_frame; check_same++) {
                        if (all_data[all_data_i].id == page_list[check_same].id) {
                            // already in the list
                            page_fault = false;
                            page_i = check_same;
                            break;
                        }
                    }

                    if (page_fault) {
                        int min_counter = 99999;
                        int min_counter_place = 0;
                        for(int i = 0; i < page_list.size(); i++) {
                            if (min_counter >= page_list[i].used_counter) {
                                min_counter = page_list[i].used_counter;
                                min_counter_place = i;
                            }
                        }
                        // erase the least counter one
                        page_list.erase(page_list.begin() + min_counter_place);

                        // insert new one
                        page_temp.used_counter++;
                        data_temp = data_list[all_data_i];
                        page_temp.id = all_data[all_data_i].id;
                        page_list.insert(page_list.begin(),page_temp);
                    }
                    else if (!page_fault) {
                        data_list[all_data_i].used_counter++;
                        page_list[page_i].used_counter++;
                    }
                }

                // put the full page list into all page list
                string page_temp;
                for (int j = 0; j < page_list.size(); j++) {
                    page_temp = page_temp + page_list[j].id;
                }

                page_history.push_back(page_temp);
                CounterUpdate(all_data[all_data_i]);
            }

        }

        void MFU(int page_frame) {
            // to do:
            // 1.get the data list
            // 2. process through the list
            // 3. check:
            // if in page frame {
            // no page fault
            // used counter + 1
            // }
            // if not in page frame {
            // page fault
            // pop the biggest counter one
            // !!! NOTE : RESET THE POP ONE'S COUNTER !!!
            // push the current one
            // }
            vector<Data> all_data = data_list; // all data
            Data data_temp; // data for push back
            Page page_temp; // page for push back

            for(int all_data_i = 0; all_data_i < all_data.size(); all_data_i++ ) {
                if (page_list.size() < page_frame) {
                    // the page is not full
                    page_temp.id = all_data[all_data_i].id;
                    page_temp.used_counter = 1;
                    data_list[all_data_i].used_counter++;
                    page_list.insert(page_list.begin(), page_temp);
                }
                
                else {
                    // the page is full
                    bool page_fault = true;

                    // check if it's in the page frame or not
                    int page_i = 0;
                    for(int check_same = 0; check_same < page_frame; check_same++) {
                        if (all_data[all_data_i].id == page_list[check_same].id) {
                            // already in the list
                            page_fault = false;
                            page_i = check_same;
                            break;
                        }
                    }

                    if (page_fault) {
                        int max_counter = -99999;
                        int max_counter_place = 0;
                        for(int i = 0; i < page_list.size(); i++) {
                            if (max_counter <= page_list[i].used_counter) {
                                max_counter = page_list[i].used_counter;
                                max_counter_place = i;
                            }
                        }
                        // erase the most counter one
                        page_list.erase(page_list.begin() + max_counter_place);
                        // reset counter
                        all_data[all_data_i].used_counter = 0;

                        // insert new one
                        all_data[all_data_i].used_counter++;
                        data_temp = data_list[all_data_i];
                        page_temp.id = all_data[all_data_i].id;
                        page_list.insert(page_list.begin(),page_temp);
                    }
                    else if (!page_fault) {
                        data_list[all_data_i].used_counter++;
                        page_list[page_i].used_counter++;
                    }
                }

                // put the full page list into all page list
                string page_temp;
                for (int j = 0; j < page_list.size(); j++) {
                    page_temp = page_temp + page_list[j].id;
                }
                page_history.push_back(page_temp);
                CounterUpdate(all_data[all_data_i]);
            }

        }

        void LFU_LRU(int page_frame) {
            // to do:
            // 1.get the data list
            // 2. process through the list
            // 3. check:
            // if in page frame {
            // no page fault
            // used counter + 1
            // pop from page list
            // re push to the list front
            // }
            // if not in page frame {
            // page fault
            // pop the smallest counter one
            // push the current one
            // }
            vector<Data> all_data = data_list; // all data
            Data data_temp; // data for push back
            Page page_temp; // page for push back

            for(int all_data_i = 0; all_data_i < all_data.size(); all_data_i++ ) {
                if (page_list.size() < page_frame) {
                    // the page is not full
                    page_temp.id = all_data[all_data_i].id;
                    page_temp.used_counter = all_data[all_data_i].used_counter;
                    page_list.insert(page_list.begin(), page_temp);
                }
                
                else {
                    // the page is full
                    bool page_fault = true;

                    // check if it's in the page frame or not
                    int page_i = 0;
                    for(int check_same = 0; check_same < page_frame; check_same++) {
                        if (all_data[all_data_i].id == page_list[check_same].id) {
                            // already in the list
                            page_fault = false;
                            page_i = check_same;
                            break;
                        }
                    }

                    if (page_fault) {
                        int min_counter = 99999;
                        int min_counter_place = 0;
                        for(int i = 0; i < page_list.size(); i++) {
                            if (min_counter >= page_list[i].used_counter) {
                                min_counter = page_list[i].used_counter;
                                min_counter_place = i;
                            }
                        }
                        // erase the least counter one
                        page_list.erase(page_list.begin() + min_counter_place);

                        // insert new one
                        all_data[all_data_i].used_counter++;
                        data_temp = data_list[all_data_i];
                        page_temp.id = all_data[all_data_i].id;
                        page_list.insert(page_list.begin(),page_temp);
                    }
                    else if (!page_fault) {
                        data_list[all_data_i].used_counter++;
                        page_list[page_i].used_counter++;
                        data_temp = data_list[all_data_i];
                        page_temp.id = all_data[all_data_i].id;
                        page_list.insert(page_list.begin(),page_temp);
                        page_list.pop_back();
                    }
                }

                // put the full page list into all page list
                string page_temp;
                for (int j = 0; j < page_list.size(); j++) {
                    page_temp = page_temp + page_list[j].id;
                }

                // doing update
                page_history.push_back(page_temp);
                CounterUpdate(all_data[all_data_i]);
                all_data = data_list;
            }

        }

        void Clear() {
            page_history.clear();
            page_list.clear();

            // clear the counter( = 0 )
            for (int i = 0; i < data_list.size(); i++) {
                data_list[i].used_counter = 0;
            }
            
        }
};
int main() {
    // get the method num and time slice
    int method_num = 0;
    int page_frame = 0;
    string file_name;

    do {
        cout << "please enter filename(e.g input1):" ;
        cin >> file_name;

        if(file_name == "end") {
            break;
        }

        ifstream get_method;
        get_method.open(file_name + ".txt");
        get_method >> method_num >> page_frame;
        get_method.close();

        Page_Replacement data_list;

        data_list.Infile(file_name + ".txt");
        data_list.Print();
        data_list.FIFO(page_frame);
        data_list.Clear();
        data_list.LRU(page_frame);
        data_list.Clear();
        data_list.LFU(page_frame);
        data_list.Clear();
        data_list.MFU(page_frame);
        data_list.Clear();
        data_list.LFU_LRU(page_frame);
        data_list.Clear();

    }while(file_name != "end");

    return 0;
}