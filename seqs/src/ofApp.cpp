
#include "ofApp.h"
#include "ofMath.h"




void ofApp::setup() {
    
    
    midiOut.openVirtualPort("ofxMidiOut");
    channel=1;
    sliderA.setup();
    sliderB.setup();
    sliderC.setup();
    sliderD.setup();
    sliderE.setup();
    sliderF.setup();
    sliderG.setup();
    sliderH.setup();
    toggleA.setup();
    knobA.setup();
    knobB.setup();
    xyA.setup(300,40);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    xyA.valueX= 100;
    xyA.valueY= 100;
    

    c=10;
   
 
   
    //ofSetLogLevel("Pd", OF_LOG_VERBOSE); // see verbose info inside

    // double check where we are ...
    cout << ofFilePath::getCurrentWorkingDirectory() << endl;

    // the number of libpd ticks per buffer,
    // used to compute the audio buffer len: tpb * blocksize (always 64)
    #ifdef TARGET_LINUX_ARM
        // longer latency for Raspberry PI
        int ticksPerBuffer = 32; // 32 * 64 = buffer len of 2048
        int numInputs = 0; // no built in mic
    #else
        int ticksPerBuffer = 8; // 8 * 64 = buffer len of 512
        int numInputs = 1;
    #endif

    // setup OF sound stream
    ofSoundStreamSettings settings;
    settings.numInputChannels = 2;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = ofxPd::blockSize() * ticksPerBuffer;
    settings.setInListener(this);
    settings.setOutListener(this);
    ofSoundStreamSetup(settings);

    // setup Pd
    //
    // set 4th arg to true for queued message passing using an internal ringbuffer,
    // this is useful if you need to control where and when the message callbacks
    // happen (ie. within a GUI thread)
    //
    // note: you won't see any message prints until update() is called since
    // the queued messages are processed there, this is normal
    //
    if(!pd.init(2, numInputs, 44100, ticksPerBuffer, false)) {
        OF_EXIT_APP(1);
    }

    midiChan = 1; // midi channels are 1-16

    // subscribe to receive source names
    pd.subscribe("toOF");
    pd.subscribe("env");

    // add message receiver, required if you want to recieve messages
    pd.addReceiver(*this); // automatically receives from all subscribed sources
    pd.ignoreSource(*this, "env");        // don't receive from "env"
    //pd.ignoreSource(*this);             // ignore all sources
    //pd.receiveSource(*this, "toOF");      // receive only from "toOF"

    // add midi receiver, required if you want to recieve midi messages
    pd.addMidiReceiver(*this); // automatically receives from all channels
    //pd.ignoreMidiChannel(*this, 1);     // ignore midi channel 1
    //pd.ignoreMidiChannel(*this);        // ignore all channels
    //pd.receiveMidiChannel(*this, 1);    // receive only from channel 1

    // add the data/pd folder to the search path
    pd.addToSearchPath("pd/abs");

    // audio processing on
    pd.start();

    // -----------------------------------------------------
    cout << endl << "BEGIN Patch Test" << endl;

    // open patch
    Patch patch = pd.openPatch("pd/seq4.pd");
    cout << patch << endl;

    // close patch
    pd.closePatch(patch);
    cout << patch << endl;

    // open patch again
    patch = pd.openPatch(patch);
    cout << patch << endl;
    
    cout << "FINISH Patch Test" << endl;

    // -----------------------------------------------------
    cout << endl << "BEGIN Message Test" << endl;

    // test basic atoms
    pd.sendBang("fromOF");
    pd.sendFloat("fromOF", 100);
    pd.sendSymbol("fromOF", "test string");

    // stream interface
    pd << Bang("fromOF")
       << Float("fromOF", 100)
       << Symbol("fromOF", "test string");

    // send a list
    pd.startMessage();
    pd.addFloat(1.23);
    pd.addSymbol("a symbol");
    pd.finishList("fromOF");

    // send a message to the $0 receiver ie $0-fromOF
    pd.startMessage();
    pd.addFloat(1.23);
    pd.addSymbol("a symbol");
    pd.finishList(patch.dollarZeroStr()+"-fromOF");

    // send a list using the List object
    List testList;
    testList.addFloat(1.23);
    testList.addSymbol("sent from a List object");
    pd.sendList("fromOF", testList);
    pd.sendMessage("fromOF", "msg", testList);

    // stream interface for list
    pd << StartMessage() << 1.23 << "sent from a streamed list" << FinishList("fromOF");

    cout << "FINISH Message Test" << endl;

    // -----------------------------------------------------
    cout << endl << "BEGIN MIDI Test" << endl;

    // send functions
    pd.sendNoteOn(midiChan, 60);
    pd.sendControlChange(midiChan, 0, 64);
    pd.sendProgramChange(midiChan, 100);    // note: pgm num range is 1 - 128
    pd.sendPitchBend(midiChan, 2000);   // note: ofxPd uses -8192 - 8192 while [bendin] returns 0 - 16383,
                                        // so sending a val of 2000 gives 10192 in pd
    pd.sendAftertouch(midiChan, 100);
    pd.sendPolyAftertouch(midiChan, 64, 100);
    pd.sendMidiByte(0, 239);    // note: pd adds +2 to the port number from [midiin], [sysexin], & [realtimein]
    pd.sendSysex(0, 239);       // so sending to port 0 gives port 2 in pd
    pd.sendSysRealTime(0, 239);

//    // stream
//    pd << NoteOn(midiChan, 60) << ControlChange(midiChan, 100, 64)
//       << ProgramChange(midiChan, 100) << PitchBend(midiChan, 2000)
//       << Aftertouch(midiChan, 100) << PolyAftertouch(midiChan, 64, 100)
//       << StartMidi(0) << 239 << Finish()
//       << StartSysex(0) << 239 << Finish()
//       << StartSysRealTime(0) << 239 << Finish();
//
//    cout << "FINISH MIDI Test" << endl;

    // -----------------------------------------------------
    cout << endl << "BEGIN Array Test" << endl;

    // array check length
    cout << "array1 len: " << pd.arraySize("array1") << endl;

    // read array
    std::vector<float> array1;
    pd.readArray("array1", array1);    // sets array to correct size
    cout << "array1 ";
    for(int i = 0; i < array1.size(); ++i)
        cout << array1[i] << " ";
    cout << endl;

    // write array
    for(int i = 0; i < array1.size(); ++i)
        array1[i] = i;
    pd.writeArray("array1", array1);

    // ready array
    pd.readArray("array1", array1);
    cout << "array1 ";
    for(int i = 0; i < array1.size(); ++i)
        cout << array1[i] << " ";
    cout << endl;

    // clear array
    pd.clearArray("array1", 10);

    // ready array
    pd.readArray("array1", array1);
    cout << "array1 ";
    for(int i = 0; i < array1.size(); ++i)
        cout << array1[i] << " ";
    cout << endl;

    cout << "FINISH Array Test" << endl;

    // -----------------------------------------------------
    cout << endl << "BEGIN PD Test" << endl;

    pd.sendSymbol("fromOF", "test");

    cout << "FINISH PD Test" << endl;

    // -----------------------------------------------------
    cout << endl << "BEGIN Instance Test" << endl;

    // open 10 instances
    for(int i = 0; i < 10; ++i) {
        Patch p = pd.openPatch("pd/instance.pd");
        instances.push_back(p);
    }

    // send a hello bang to each instance individually using the dollarZero
    // to [r $0-instance] which should print the instance dollarZero unique id
    // and a unique random number
    for(int i = 0; i < instances.size(); ++i) {
        pd.sendBang(instances[i].dollarZeroStr()+"-instance");
    }

    // send a random float between 0 and 100
    for(int i = 0; i < instances.size(); ++i) {
        pd.sendFloat(instances[i].dollarZeroStr()+"-instance", int(ofRandom(0, 100)));
    }

    // send a symbol
    for(int i = 0; i < instances.size(); ++i) {
        pd.sendSymbol(instances[i].dollarZeroStr()+"-instance", "howdy dude");
    }

    // close all instances
    for(int i = 0; i < instances.size(); ++i) {
        pd.closePatch(instances[i]);
    }
    instances.clear();

    cout << "FINISH Instance Test" << endl;
     
    pd.sendBang("sup");

}

//--------------------------------------------------------------

void ofApp::update() {
    
      
    
     
     sliderA.update();
     sliderB.update();
     sliderC.update();
     sliderD.update();
     sliderE.update();
     sliderF.update();
     sliderG.update();
     sliderH.update();
     toggleA.update();
    
     knobA.update();
    xyA.update();
    // since this is a test and we don't know if init() was called with
    // queued = true or not, we check it here
    if(pd.isQueued()) {
        // process any received messages, if you're using the queue and *do not*
        // call these, you won't receieve any messages or midi!
        pd.receiveMessages();
        pd.receiveMidi();
    }

    
}

//--------------------------------------------------------------

void ofApp::draw() {
    ofBackground(152, 150, 162);
    t = t+1;
    size=2;
    
  
   
    ofDrawBitmapString("phase conductor", 30, 250);
    ofDrawBitmapString("///////////////////////////", 30, 270);

  
    sliderA.draw();
    sliderB.draw();
    sliderC.draw();
    sliderD.draw();
    sliderE.draw();
    sliderF.draw();
    sliderG.draw();
    sliderH.draw();
    
    xyA.draw(325, 40);
    
    toggleA.draw();
    toggleA.xPos=385;
    toggleA.base=200;
 
    knobA.draw(325,260,12);
    knobB.draw(450,260,12);
    
    sliderA.xPos = 40;
    sliderA.base = 100;
    sliderB.xPos = 100;
    sliderB.base = 100;
    sliderC.xPos = 160;
    sliderC.base = 100;
    sliderD.xPos = 220;
    sliderD.base = 100;
    sliderE.xPos = 40;
    sliderE.base = 200;
    sliderF.xPos = 100;
    sliderF.base = 200;
    sliderG.xPos = 160;
    sliderG.base = 200;
    sliderH.xPos = 220;
    sliderH.base = 200;

    
    std::vector<float> array1;
    pd.readArray("array1", array1);    // sets array to correct size
    cout << "array1 ";
    for(int i = 0; i < array1.size(); ++i)
        cout << array1[i] << " ";
    cout << endl;
    
    array1[0]=sliderA.value;
    array1[1]=sliderB.value;
    array1[2]=sliderC.value;
    array1[3]=sliderD.value;
    
    pd.writeArray("array1",array1);
    
    std::vector<float> array2;
    pd.readArray("array2", array2);    // sets array to correct size
    cout << "array2";
    for(int i = 0; i < array2.size(); ++i)
        cout << array2[i] << " ";
    cout << endl;
    
    array2[0]=sliderE.value;
    array2[1]=sliderF.value;
    array2[2]=sliderG.value;
    array2[3]=sliderH.value;
    
    pd.writeArray("array2",array2);
    
    pd.sendFloat("transpose", knobA.value);
    pd.sendFloat("transposeB", knobB.value);
    pd.sendFloat("rate", xyA.valueX);
    pd.sendFloat("clock", xyA.valueY);
    pd.sendFloat("tick", xyA.valueY);
    
    
    if (toggleA.value == true){
        
        pd.sendFloat("sup", 1);
    }
    else( pd.sendFloat("sup", 0));

   
    if ( yep ==true){
    
    midiOut.sendNoteOn(chan,note,vel);
   
    }
    
    else {yep==false;}
    
    float newTx = ofMap(xyA.valueX, 0, 1000, 1, 0);
    float newTy = ofMap(xyA.valueY, 0, 1000, 1, 0);
    
    ofPushMatrix();
    ofTranslate(250,500,0);
    ofRotateZDeg(newTx*t*20);
    ofSetColor(4, 228, 165);
    sphere.setRadius(sliderA.value*2+size);
    sphereB.setRadius(sliderB.value*2+size);
    sphere.setPosition(-150, 0, 0);
    sphereB.setPosition(-50, 0, 0);
    sphere.draw();
    sphereB.draw();
    sphereC.setRadius(sliderC.value*2+size);
    sphereD.setRadius(sliderD.value*2+size);
    sphereC.setPosition(50, 0, 0);
    sphereD.setPosition(150, 0, 0);
    sphereC.draw();
    sphereD.draw();
    ofPopMatrix();
        
    ofPushMatrix();
    ofTranslate(250,500,0);
    ofRotateYDeg(newTy*t*20);
    ofSetColor(0);
    sphereE.setRadius(sliderE.value*2+size);
    sphereF.setRadius(sliderF.value*2+size);
    sphereE.setPosition(-150, 0, 0);
    sphereF.setPosition(-50, 0, 0);
    sphereE.draw();
    sphereF.draw();
    sphereG.setRadius(sliderG.value*2+size);
    sphereH.setRadius(sliderH.value*2+size);
    sphereG.setPosition(50, 0, 0);
    sphereH.setPosition(150, 0, 0);
    sphereG.draw();
    sphereH.draw();
    ofPopMatrix();
   
    
}

//--------------------------------------------------------------
void ofApp::exit() {

    // cleanup
    midiOut.closePort();
    ofSoundStreamStop();
}

//--------------------------------------------------------------
void ofApp::playTone(int pitch) {
    pd << StartMessage() << "pitch" << pitch << FinishList("tone") << Bang("tone");
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {

    switch(key) {
        
        // musical keyboard
        case 'a':
            playTone(60);
            break;
        case 'w':
            playTone(61);
            break;
        case 's':
            playTone(62);
            break;
        case 'e':
            playTone(63);
            break;
        case 'd':
            playTone(64);
            break;
        case 'f':
            playTone(65);
            break;
        case 't':
            playTone(66);
            break;
        case 'g':
            playTone(67);
            break;
        case 'y':
            playTone(68);
            break;
        case 'h':
            playTone(69);
            break;
        case 'u':
            playTone(70);
            break;
        case 'j':
            playTone(71);
            break;
        case 'k':
            playTone(72);
            break;

        case ' ':
            if(pd.isReceivingSource(*this, "env")) {
                pd.ignoreSource(*this, "env");
                cout << "ignoring env" << endl;
            }
            else {
                pd.receiveSource(*this, "env");
                cout << "receiving from env" << endl;
            }
            break;

        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
    pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
    pd.audioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::print(const std::string& message) {
    cout << message << endl;
}

//--------------------------------------------------------------
void ofApp::receiveBang(const std::string& dest) {
    cout << "OF: bang " << dest << endl;
    
    if (dest == "ofbang"){
    yep = true;
    }
    
}

void ofApp::receiveFloat(const std::string& dest, float value) {
    cout << "OF: float " << dest << ": " << value << endl;
}

void ofApp::receiveSymbol(const std::string& dest, const std::string& symbol) {
    cout << "OF: symbol " << dest << ": " << symbol << endl;
}

void ofApp::receiveList(const std::string& dest, const List& list) {
    cout << "OF: list " << dest << ": ";

    // step through the list
    for(int i = 0; i < list.len(); ++i) {
        if(list.isFloat(i))
            cout << list.getFloat(i) << " ";
        else if(list.isSymbol(i))
            cout << list.getSymbol(i) << " ";
    }

    // you can also use the built in toString function or simply stream it out
    // cout << list.toString();
    // cout << list;

    // print an OSC-style type string
    cout << list.types() << endl;
}

void ofApp::receiveMessage(const std::string& dest, const std::string& msg, const List& list) {
    cout << "OF: message " << dest << ": " << msg << " " << list.toString() << list.types() << endl;
}

//--------------------------------------------------------------
void ofApp::receiveNoteOn(const int channel, const int pitch, const int velocity) {
    cout << "OF MIDI: note on: " << channel << " " << pitch << " " << velocity << endl;
   
    chan=channel;
    note=pitch;
    vel=velocity;
    midiOut.sendNoteOn(chan,note,vel);
    
}

void ofApp::receiveControlChange(const int channel, const int controller, const int value) {
    cout << "OF MIDI: control change: " << channel << " " << controller << " " << value << endl;
}

// note: pgm nums are 1-128 to match pd
void ofApp::receiveProgramChange(const int channel, const int value) {
    cout << "OF MIDI: program change: " << channel << " " << value << endl;
}

void ofApp::receivePitchBend(const int channel, const int value) {
    cout << "OF MIDI: pitch bend: " << channel << " " << value << endl;
}

void ofApp::receiveAftertouch(const int channel, const int value) {
    cout << "OF MIDI: aftertouch: " << channel << " " << value << endl;
}

void ofApp::receivePolyAftertouch(const int channel, const int pitch, const int value) {
    cout << "OF MIDI: poly aftertouch: " << channel << " " << pitch << " " << value << endl;
}

// note: pd adds +2 to the port num, so sending to port 3 in pd to [midiout],
//       shows up at port 1 in ofxPd
void ofApp::receiveMidiByte(const int port, const int byte) {
    cout << "OF MIDI: midi byte: " << port << " " << byte << endl;
}

