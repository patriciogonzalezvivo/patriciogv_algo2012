#include "testApp.h"
#include "Utils.h"
#include <stdio.h>
#include "ofConstants.h"

void convertWindowsToUnixPath(string & path){
    for (int i = 0; i < path.size(); i++){
        if (path[i] == '\\') path[i] = '/';
    }
}

string windowsFromUnixPath(string path){
    for (int i = 0; i < path.size(); i++){
        if (path[i] == '/') path[i] = '\\';
    }
	return path;
}

void extractFolderFromPath(string &_path, string &_folder){
    string completePath = _path;
    _folder = "";
    _path = "";
    
    int i;
    for (i = completePath.size()-1 ; completePath[i] != '/'; i--){
        _folder.insert(_folder.begin(), completePath[i]);
    }
    for (i-- ; completePath[i] >= 0; i--){
        _path.insert(_path.begin(), completePath[i]);
    }
}

void fixStringCharacters(string &toFix){
    
    // replace all non alpha numeric (ascii) characters with _
    for (int i = 0; i < toFix.size(); i++){
        int which = (int)toFix[i];
        if ((which >= 48 && which <= 57) ||
            (which >= 65 && which <= 90) ||
            (which >= 97 && which <= 122)){
        } else {
            toFix[i] = '_';
        }
    }
}

bool isProjectFolder(string &_projFolder){
    //  Return true or false if a project Folder structure it's found and change the _projFolder string
    //  to become the correct path to a folder structure
    //
    
    //  1. If is a directory
    //
    ofDirectory dir;
    string searchFor = _projFolder;
    dir.open(searchFor);
    if ( dir.isDirectory() ){
        
        //  Is a project directory or a src directory?
        //
        string folder;
        extractFolderFromPath(searchFor, folder);
        if ( (folder == "src") || (folder == "bin") || ( (int)folder.find(".xcodeproj") > 0) ){
            _projFolder = searchFor;
        } else {
            searchFor = _projFolder;
        }
        
    } else {
        
        //  If is a file it have something related to a project?
        //
        string name;
        extractFolderFromPath(searchFor, name);
        if (((int)name.find(".cbp") > 0) ||
            ((int)name.find(".workspace") > 0) ||
            ((int)name.find(".plist") > 0) ||
            ((int)name.find(".xcconfig") > 0) ||
            ((int)name.find(".make") > 0) ||
            ((int)name.find(".vcxproj") > 0 )){
            _projFolder = searchFor;
        } else {
            return false;
        }
    }
    
    //  3. Have src/
    //
    searchFor = searchFor+"/src";
    dir.open( searchFor );
    if (!dir.isDirectory())
        return false;
    
    //  4. Have main.cpp, testApp.h, testApp.cpp?
    //
    ofFile test;
    bool    isMainCpp = test.open(searchFor+"/main.cpp");
    bool    isTestAppH = test.open(searchFor+"/testApp.h");
    bool    isTestAppCpp = test.open(searchFor+"/testApp.cpp");
    
    if ( !(isMainCpp && isTestAppH && isTestAppCpp) )
        return false;
    
    return true;
}

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //  Default Settings
    //
    mode                = MODE_NORMAL;
    project             = NULL;
    statusEnergy        = 0;
    float paddingTop    = 64;
    float paddingButton = 20;
    float paddingLeft   = 64;
    float paddingRight  = 20;
    string  sketchName  = "mySketch";
    
    ofBackground(230,230,230);
    
    //  XML Settings
    //
    ofxXmlSettings XML;
    XML.loadFile("settings/projectGeneratorSettings.xml");
    appToRoot = XML.getValue("appToRoot", "../../../../");
    defaultLoc = XML.getValue("defaultNewProjectLocation", "apps/myApps");
    
    //-------------------------------------
    // calculate the bin path (../../../ on osx) and the sketch path (bin -> root - > defaultLoc)
    //-------------------------------------
    // if appToRoot is wrong, we have alot of issues.  all these paths are used in this project:
    //
#ifdef TARGET_OSX
    string binPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofFilePath::getCurrentWorkingDirectory(), "../../../"));
#else
    string binPath = ofFilePath::getCurrentExeDir();
#endif
    
    string ofRoot = ofFilePath::getAbsolutePath(ofFilePath::join(binPath, appToRoot));
    addonsPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot,"addons"));
    string sketchPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot, defaultLoc));
    
    convertWindowsToUnixPath(ofRoot);
    convertWindowsToUnixPath(addonsPath);
    convertWindowsToUnixPath(sketchPath);
    
    // there's some issues internally in OF with non unix paths for OF root
    //
    setOFRoot(ofRoot);
    
    //------------------------------------- GUI
    //
    
    //  load font and setup the buttons
    //
    font.loadFont("fonts/Inconsolata.otf", 14, true,false,false,0.5,90);
    secondFont.loadFont("fonts/GeosansLight.ttf", 11, true,false,false,0.5,90);
    
    //  Sketch button
    //
    textButton  *buttonName = new textButton();
    buttonName->font = &font;
    buttonName->secondFont = &secondFont;
    buttonName->prefix = "Name: ";
    buttonName->enableEditing();
	buttonName->topLeftAnchor.set(paddingTop, paddingLeft); //set top button position - others are set relative to this.
    buttonName->setText(sketchName);
    buttonName->secondaryText = "<< CLICK TO CHANGE THE NAME";
    buttons.push_back(buttonName);
    
    //  Path Button
    //
    textButton  *buttonPath = new textButton();
    buttonPath->font = &font;
    buttonPath->secondFont = &secondFont;
    buttonPath->topLeftAnchor.set(buttons[ buttons.size() -1 ]->topLeftAnchor.x, buttons[ buttons.size() -1 ]->topLeftAnchor.y + buttons[ buttons.size() -1 ]->height + paddingButton);
    buttonPath->deliminater = "/";
    buttonPath->prefix = "Path: ";
    buttonPath->setText(sketchPath);
    buttonPath->secondaryText = "<< CLICK TO CHANGE THE DIRECTORY";
    buttons.push_back(buttonPath);
    
    //  Platform Button
    //
    textButton  *buttonPlat = new textButton();
    buttonPlat->font = &font;
    buttonPlat->secondFont = &secondFont;
    buttonPlat->topLeftAnchor.set(buttons[ buttons.size() -1 ]->topLeftAnchor.x, buttons[ buttons.size() -1 ]->topLeftAnchor.y + buttons[ buttons.size() -1 ]->height + paddingButton);
    buttonPlat->deliminater = ", ";
    buttonPlat->prefix = "Platforms: ";
    buttonPlat->secondaryText = "<< CLICK TO CHANGE THE PLATFORM";
    buttonPlat->setText("");
    buttons.push_back(buttonPlat);
    
    //  Addons Button
    //
    textButton  *buttonAddon = new textButton();
    buttonAddon->font = &font;
    buttonAddon->secondFont = &secondFont;
    buttonAddon->topLeftAnchor.set(buttons[ buttons.size() -1 ]->topLeftAnchor.x, buttons[ buttons.size() -1 ]->topLeftAnchor.y + buttons[ buttons.size() -1 ]->height + paddingButton);
    buttonAddon->deliminater = ", ";
    buttonAddon->prefix = "Addons: ";
    buttonAddon->secondaryText = "<< CLICK TO SELECT ADDONS";
    buttonAddon->setText("");
    buttons.push_back(buttonAddon);
    
    for (int i = 0; i < buttons.size(); i++){
        buttons[i]->calculateRect();
    }
    
    //  Generate Button
    //
    generateButton.font = &font;
    generateButton.secondFont = &secondFont;
    generateButton.deliminater = ",";
    generateButton.prefix = "GENERATE PROJECT";
    generateButton.setText("");
    generateButton.bDrawLong = false;
    generateButton.topLeftAnchor.set(ofGetWidth() - buttons[0]->x - generateButton.width + paddingRight , ofGetHeight() - generateButton.height - paddingButton);
    generateButton.calculateRect();
    
    //  Addon Button
    //
    backButton = generateButton;
    backButton.prefix = "BACK >>";
    backButton.setText("");
    backButton.bDrawLong = false;
    backButton.calculateRect();
    
    //  LOAD ADDONS
    //
    coreAddonsList.set(paddingLeft-12,paddingTop-22,270,500);
    coreAddonsList.font = &font;
    coreAddonsList.secondFont = &secondFont;
    coreAddonsList.title = "CORE ADDONS";
    otherAddonsList.set(coreAddonsList.x+coreAddonsList.width+paddingRight,coreAddonsList.y,coreAddonsList.width,coreAddonsList.height);
    otherAddonsList.font = &font;
    otherAddonsList.secondFont = &secondFont;
    otherAddonsList.title = "EXTRA ADDONS";
    
    ofDirectory addonsFolder(addonsPath);
    addonsFolder.listDir();
    for(int i=0; i < (int)addonsFolder.size();i++){
    	string addonName = addonsFolder.getName(i);
        
    	if(addonName.find("ofx")==0){
            if (isAddonCore(addonName)){
                coreAddonsList.addElement(addonName);
            } else {
                bHaveNonCoreAddons = true;
                otherAddonsList.addElement(addonName);
            }
    	}
    }
    
    //  LOAD PLATFORMS
    //
    platformsList.set(paddingLeft-12,paddingTop-22,270,500);
    platformsList.font = &font;
    platformsList.secondFont = &secondFont;
    platformsList.title = "PLATFORM TARGETS";
    platformsList.addElement("windows (codeblocks)",ofGetTargetPlatform()==OF_TARGET_WINGCC);
	platformsList.addElement("windows (visualStudio)", ofGetTargetPlatform()==OF_TARGET_WINVS);
	platformsList.addElement("linux (codeblocks)",ofGetTargetPlatform()==OF_TARGET_LINUX);
	platformsList.addElement("linux64 (codeblocks)",ofGetTargetPlatform()==OF_TARGET_LINUX64);

    //#define MAKE_IOS
#ifdef MAKE_IOS
	platformsList.addElement("osx (xcode)",false);
	platformsList.addElement("ios (xcode)",true);
#else
    platformsList.addElement("osx (xcode)",ofGetTargetPlatform()==OF_TARGET_OSX);
	platformsList.addElement("ios (xcode)",ofGetTargetPlatform()==OF_TARGET_IPHONE);
#endif
    
    // update the platforms text in the platform button
    //
    buttons[2]->setText( platformsList.getSelectedAsString() );
    
    guiEl.text = " esto es un texto largo ";
    guiEl.prefix = "Path:";
    guiEl.deliminater = "/";
    guiEl.bLeftAlign = false;
    guiEl.setFont(font);
    guiEl.setSizeAndShapes(38, 3, 4);
    guiEl.x = ofGetWidth()*0.5;
    guiEl.y = ofGetHeight()*0.5;
    guiEl.enable();
//    guiEl.width = 300;
    
}

//------------------------------------------------------
bool testApp::isAddonCore(string addon){
    //  Pre define what's a core addon
    //
    vector<string>  coreAddons;
    coreAddons.push_back("ofx3DModelLoader");
    coreAddons.push_back("ofxAssimpModelLoader");
    coreAddons.push_back("ofxDirList");
    coreAddons.push_back("ofxNetwork");
    coreAddons.push_back("ofxOpenCv");
    coreAddons.push_back("ofxOsc");
    coreAddons.push_back("ofxThread");
    coreAddons.push_back("ofxThreadedImageLoader");
    coreAddons.push_back("ofxVectorGraphics");
    coreAddons.push_back("ofxVectorMath");
    coreAddons.push_back("ofxXmlSettings");
    coreAddons.push_back("ofxSvg");
    
    for (int i = 0; i < coreAddons.size(); i++){
        if (coreAddons[i] == addon){
            return true;
        }
    }
    return false;
}

void testApp::loadProject(string _path){
    //  Extract Name and Path
    //
    string folder = "";
    
    extractFolderFromPath(_path,folder);
    buttons[0]->setText(folder);
    buttons[1]->setText(_path);
    setStatus("Project " + folder + " loaded ");
    
    //  Extracting Addons ( from addons.make)
    //
    coreAddonsList.reset();
    otherAddonsList.reset();
    
    //  Have addons.make??
    //
    ofFile test;
    bool    isAddons = test.open(_path + "/" + folder + "/addons.make");
    if ( !isAddons )
        return;
    
    //  Add addons
    //
    ifstream fs( (_path + "/" + folder + "/addons.make").c_str());
    int counter = 0;
    string line;
    string addonsAdded = "";
    while(!(fs >> line).fail()){
        
        if ( selectAddon(line) ){
            if (counter > 0)
                addonsAdded +=", ";
            addonsAdded += line;
        } else {
            cout << "Error: loading " << line << endl;
        }
        counter++;
    }
    fs.seekg(0,ios::beg);
    fs.clear();
    fs.close();
    
    buttons[3]->setText(addonsAdded);
}

bool testApp::selectAddon(string _addonName){
    if ( isAddonCore(_addonName)){
        coreAddonsList.select(_addonName);
    } else {
        otherAddonsList.select(_addonName);
    }
}

string testApp::setTarget(int targ){
    
    if(project){
		delete project;
	}
    
    string target;
    switch(targ){
        case OF_TARGET_OSX:
            project = new xcodeProject;
            target = "osx";
            break;
        case OF_TARGET_WINGCC:
            project = new CBWinProject;
            target = "win_cb";
            break;
        case OF_TARGET_WINVS:
            project = new visualStudioProject;
            target = "vs2010";
            break;
        case OF_TARGET_IPHONE:
            project = new xcodeProject();
            target = "ios";
            break;
        case OF_TARGET_ANDROID:
            break;
        case OF_TARGET_LINUX:
            project = new CBLinuxProject;
            target = "linux";
            break;
        case OF_TARGET_LINUX64:
            project = new CBLinuxProject;
            target = "linux64";
            break;
    }
    
    project->setup(target);
    return target;
}

void testApp::setStatus(string newStatus){
    statusEnergy = 1;
    status = newStatus;
    statusSetTime = ofGetElapsedTimef();
}

void testApp::generateProject(){
    
    vector <int> targetsToMake;
    for(int i = 0; i < platformsList.elements.size(); i++){
        if ( *(platformsList.elements[i]) == true ){
            if (platformsList.elements[i]->text == "windows (codeblocks)" ){
                targetsToMake.push_back(OF_TARGET_WINGCC);
            } else if (platformsList.elements[i]->text == "windows (visualStudio)"){
                targetsToMake.push_back(OF_TARGET_WINVS);
            } else if (platformsList.elements[i]->text == "linux (codeblocks)"){
                targetsToMake.push_back(OF_TARGET_LINUX);
            } else if (platformsList.elements[i]->text == "linux64 (codeblocks)"){
                targetsToMake.push_back(OF_TARGET_LINUX64);
            } else if (platformsList.elements[i]->text == "osx (xcode)"){
                targetsToMake.push_back(OF_TARGET_OSX);
            } else if (platformsList.elements[i]->text == "ios (xcode)"){
                targetsToMake.push_back(OF_TARGET_IPHONE);
            }
        }
    }
    
    cout << targetsToMake.size() << endl;
    
	if( targetsToMake.size() == 0 ){
		cout << "Error: makeNewProjectViaDialog - must specifiy a project to generate " <<endl;
		ofSystemAlertDialog("Error: makeNewProjectViaDialog - must specifiy a project platform to generate");
        return;
	}
    
    if (buttons[0]->text.size() == 0){
        ofSystemAlertDialog("Error: project must have a name");
        return;
    }
    
    printf("start with project generation \n");
    
    string path = ofFilePath::join(buttons[1]->text, buttons[0]->text);
    
	for(int i = 0; i < (int)targetsToMake.size(); i++){
		string target = setTarget(targetsToMake[i]);
        
        if(project->create(path)){
            
            vector<string> addons = coreAddonsList.getSelected(); 
            for (int i = 0; i < addons.size(); i++){
                ofAddon addon;
                addon.pathToOF = getOFRelPath(path);
                addon.fromFS(ofFilePath::join(addonsPath, addons[i]),target);
                project->addAddon(addon);
            }
            
            addons = coreAddonsList.getSelected();
            for (int i = 0; i < addons.size(); i++){
                ofAddon addon;
                addon.pathToOF = getOFRelPath(path);
                addon.fromFS(ofFilePath::join(addonsPath, addons[i]),target);
                project->addAddon(addon);
            }
            
            project->save(true);
        }
        
        setStatus("generated: " + buttons[1]->text + "/" + buttons[0]->text + " for " + platformsList.getSelected()[i]);
	}
    
    printf("done with project generation \n");
}

//--------------------------------------------------------------
void testApp::update(){
    ofPoint mouse = ofPoint(mouseX, mouseY);
    
    float diff = ofGetElapsedTimef()- statusSetTime;
    if (diff > 3){
        statusEnergy *= 0.99;;
    }
    //-------------------------------------
    // if we are in addon mode check
    //-------------------------------------

    if (mode == MODE_NORMAL){
        for (int i = 0; i < buttons.size(); i++){
            buttons[i]->calculateRect();
            buttons[i]->checkMousePressed(mouse);
        }
        generateButton.checkMousePressed(mouse);
        
        for (int i = 0; i < buttons.size(); i++){
            if (i != 0){
                buttons[i]->topLeftAnchor.y = buttons[i-1]->topLeftAnchor.y + buttons[i-1]->height + 20;
            }
        }
        
    } else if (mode == MODE_ADDON ){
        backButton.checkMousePressed(mouse);
        coreAddonsList.update();
        otherAddonsList.update();
    } else if (mode == MODE_PLATFORM){
        backButton.checkMousePressed(mouse);
        platformsList.update();
    }

}

//--------------------------------------------------------------
void testApp::draw(){
    
    if (mode == MODE_NORMAL ) {
		for (int i = 0; i < buttons.size(); i++){
			buttons[i]->draw();
		}
        generateButton.draw();
        
    } else if (mode == MODE_ADDON){
        coreAddonsList.draw();
        if (bHaveNonCoreAddons){
            otherAddonsList.draw();
        }
        backButton.draw();
        
    } else if (mode == MODE_PLATFORM){
        platformsList.draw();
        backButton.draw();
    }
    
    ofFill();
    ofSetColor(0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy));
    ofRect(0,ofGetHeight(), ofGetWidth(), -25);
    ofSetColor(255,255,255, 255 * statusEnergy);
    ofDrawBitmapString(status, 10,ofGetHeight()-8);
    
    guiEl.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    ofPoint mouse = ofPoint(x, y);
    if (mode == MODE_NORMAL){

        for (int i = 0; i < buttons.size(); i++){
            buttons[i]->checkMousePressed(mouse);
        }
        
        //-------------------------------------
        // 0 = sketch name
        //-------------------------------------
//        if (buttons[0].bMouseOver == true){
//            string text = ofSystemTextBoxDialog("choose sketch name", buttons[0].text);
//            fixStringCharacters(text);
//            setStatus("sketch name set to: " + text);
//            buttons[0].setText(text);
//        }

        //-------------------------------------
        // 1 = sketch path
        //-------------------------------------
        if (buttons[1]->bMouseOver == true){

            string command = "";

            ofDirectory dir(ofFilePath::join(getOFRoot(),defaultLoc));

            if (!dir.exists()){
                dir.create();
            }
          	
        #ifdef TARGET_WIN32
                    ofFileDialogResult res = ofSystemLoadDialog("please select sketch folder", true, windowsFromUnixPath(dir.path()));
        #else 
                    ofFileDialogResult res = ofSystemLoadDialog("please select sketch folder", true, dir.path());
        #endif
            

            if (res.bSuccess){
                string result = res.filePath;
                convertWindowsToUnixPath(result);
                buttons[1]->setText( result );
                
                setStatus("path set to: " + result);
            }
        }

        //-------------------------------------
        // 2 = platform  (disabled)
        //-------------------------------------
        if (buttons[2]->bMouseOver == true){
            // platform is diabled for now
             mode = MODE_PLATFORM;
        }

        //-------------------------------------
        // 3 = addon
        //-------------------------------------
        if (buttons[3]->bMouseOver == true){
            mode = MODE_ADDON;
        }
        
        //-------------------------------------
        // 4 = genearate
        //-------------------------------------
        if (generateButton.bMouseOver == true){
            generateProject();
        }
    } else if (mode == MODE_ADDON ){
        
        coreAddonsList.checkMousePressed(mouse);
        if(bHaveNonCoreAddons)
            otherAddonsList.checkMousePressed(mouse);
        
        //-------------------------------------
        // if we hit he back button, collect the addons for display
        //-------------------------------------

        if (backButton.bMouseOver){
            string addons = coreAddonsList.getSelectedAsString() + otherAddonsList.getSelectedAsString();
            buttons[3]->setText(addons);
            setStatus("addons set to: " + addons);

            backButton.bMouseOver = false;
            mode = MODE_NORMAL;
        }
    } else if (mode == MODE_PLATFORM){
        platformsList.checkMousePressed(mouse);
        
        if (backButton.bMouseOver){
            string platforms = platformsList.getSelectedAsString();
            buttons[2]->setText( platforms );
            setStatus("Platform targets set to: " + platforms);
            
            backButton.bMouseOver = false;
            mode = MODE_NORMAL;
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    generateButton.topLeftAnchor.set(ofGetWidth() - buttons[0]->x - generateButton.width + 10, ofGetHeight() - generateButton.height - 40);
    generateButton.calculateRect();
    
    backButton.topLeftAnchor.set(ofGetWidth() - buttons[0]->x - backButton.width + 10, ofGetHeight() - backButton.height - 40);
    backButton.calculateRect();
    
    coreAddonsList.set(coreAddonsList.x,coreAddonsList.y,coreAddonsList.width,ofGetHeight()-64*2-10);
    otherAddonsList.set(otherAddonsList.x,otherAddonsList.y,otherAddonsList.width, coreAddonsList.height);
    platformsList.set(platformsList.x,platformsList.y,platformsList.width,coreAddonsList.height);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    if (dragInfo.files.size() > 1)
        for (int i = 0; i < dragInfo.files.size(); i++){
            cout << dragInfo.files[i] << endl;
    } else if (dragInfo.files.size() == 1) {
        
        string open = dragInfo.files[0];
        if (isProjectFolder(open)){
            loadProject(open);
        }
    }
}
