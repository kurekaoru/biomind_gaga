    #include <iostream>
    #include <string>
    #include <thread> //threading class
    using namespace ost; //namespace for gnu common c++ library
     
    class progress : public Thread { //every threaded class must be inherited from Thread class
        int percent; //member variable holding the percentage, 0-100
    public:
        progress() : percent(0) { start(); } //initialize value for percent and start the thread now
        void run(); //function responsible for displaying and updating the progress bar
        void setPercent(const int& pct) { //member function responsible for setting value to percentage variable
            this->percent = pct;
        }
    };
     
    void progress::run() {
        std::string bar;
        while(percent <= 100) {
            for(int i=0; i < 20; i++) { //the progress bar has 20 blocks
                if(i <= percent/5) //add enough = based on the percentage
                    bar.replace(i, 1, "=");
                else
                    bar.replace(i, 1, " "); //add the spaces for empty area
            }
            std::cout<<"\r["<<bar<<"]"; //print the progress bar
            std::cout.width(4); //output 4 characters, in the next output to cout, one for space, and three for the numbers
            std::cout<<percent<<"%"<<std::flush;
            if(percent==100) break; //break when reaching 100%
            Thread::sleep(500); //interval between updates, in miliseconds
        }
        std::cout<<std::endl;
        //when the run function is ended, this thread is over
    }
     
    /* Test Program */
    #include <cmath> //for ceil function to round up the percentage
    int main() {
        progress pbar; //create a progressbar thread, it starts showing from now
        //now a time consuming job
        for(int i=1; i<100000000; i++) {
            pbar.setPercent( ceil((double)i/1000000) ); //set the  percent variable of the progressbar
        }
        pbar.join(); //wait till pbar thread ends, if it hasn't yet
        return 0;
    } 


