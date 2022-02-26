#include "RS.h"

void initSrl(){
    Serial.begin(115200);
    //attachInterrupt(0,handleRxInterrupt,CHANGE);
}

void addToSrl(String inStr){
    srlBuffer+=inStr;
    srlBuffer+="\nDON>";
}

void printSrlBuffer(){
    if (srlBuffer != "") {
        Serial.print(srlBuffer);
        srlBuffer = "";
    }
}

// Note that the "ICACHE_RAM_ATTR" type forces the event function to be loaded in IRAM, to avoid "ISR not in IRAM" exceptions;
void ICACHE_RAM_ATTR serialEvent(){
    // ---------------------BEWARE -----------------------
    // -- NEVER print to serial during the serial event --
    // -- Append the content to srlBuffer instead       --
    // ---------------------------------------------------

    char inChar = '\0';
    // Read, and returns the input character
    inChar = Serial.read();
    srlBuffer += inChar;

    // While the user doesn't terminate
    if (inChar != '\n') {
        // If the character is a "backspace":
        //  deletes the last char of inStr, in the terminal
        //  replaces the last character of the terminal by a space and displaces the carriage by one char
        if (inChar == '\b'){
            inStr = inStr.substring(0, inStr.length()-1);
            srlBuffer+=' ';
            srlBuffer+='\b';
        }
        // If the input is a valid ascii character, append
        else if (isAscii(inChar)){
            inStr += inChar;
        }
    }
    // If the user sends a Newline, the command is parsed and the buffer reinitialized
    //  The outputted command parameters will be executed on main() at the next loop iteration
    else {
        parseInput(inStr, p_outputPars);
        inStr="";
    }
}

void parseInput(String rawStr, char *outputParams){
    // Note that the output will be given in the adress pointer by **outputParams

    char rawCharArray[rawStr.length()];
    int currentParam = 0;
    int currentChar = 0;
    // Because the first character is required to start the loop
    // stringIterator begins at 1
    int stringIterator = 0;
    
    // Le "token" correspond à une chaine de caractères entre deux séparateurs
    char readedChar;
    // Conversion en charArray
    rawStr.toCharArray(rawCharArray,rawStr.length());

    // Begins with an "unkown character" to start the loop
    readedChar = '?';
    // Tant que l'on arrive pas à la fin d'une chaine de caractères ou d'une ligne
    while (readedChar != '\0' && readedChar != '\n'){

        // Reads current character
        readedChar = rawCharArray[stringIterator];

        // If valid character
        if (readedChar != ' ' && isascii(readedChar) ) {
            // Adds the new character, than terminates the string ('\0' will be replaced if next iteration)
            // Because the output is written in a pointer to bidimensionnal array:
            //  the used syntax is *(pointer + (row * number_cols) + col)
            *(outputParams + ((currentParam * MAX_PARAM_SIZE) + currentChar)) = readedChar;
            *(outputParams + ((currentParam * MAX_PARAM_SIZE) + currentChar+1)) = '\0';
            currentChar++;
        }
        // Si on lit un séparateur, on passe au paramètre suivant
        else if (readedChar == ' ' || readedChar == ';'){
            // End the string by adding a NULL TERMINATED CHARACTER
            currentParam++;
            currentChar = 0;
        }
        // On passe au caractère d'entrée suivant
        stringIterator++;
    }
}

void execCommands(char *paramList){
    // First parameter contains the instruction to execute
    char firstParam[MAX_PARAM_SIZE];
    char currentParam[MAX_PARAM_SIZE];

    // The output of the execution command
    String outputBuf;
    // Temporary buffer used to format commands
    String tempBuf;

    // Status of the execution
    bool execStatus = false;

    int i,j;

    if ((*paramList) != 0) {

        // The first parameter begins at the first adress of paramList
        memcpy(firstParam, paramList, MAX_PARAM_SIZE);

        // #############################################################################
        // ###                            HELP COMMAND                               ###
        // #############################################################################
        
        if (!strcmp(firstParam,"HELP")){
            outputBuf+="---------- HELP SECTION -----------\n";
            outputBuf+="[RST] Resets the µc\n";
            outputBuf+="[IP LOCAL|GATEWAY|SUBNET  www xxx yyy zzz]: Changes the ip configuration to www.xxx.yyy.zzz\n";
            outputBuf+="[RELAY xxx] Changes the Relay state: ON or OFF\n";
            outputBuf+="[MD ID] Displays the value in EEPROM of the parameter ID \n";
            outputBuf+="[MM ID VALUE] Modifies the value in EEPROM of the parameter ID to VALUE\n";
            outputBuf+="[WIFI_INIT] Forces the initialisation of WIFI client\n";
            outputBuf+="[MQTT_INIT] Forces the initialisation of MQTT client\n";
            outputBuf+="[MQTT_SUB xxx] Subsribes to MQTT topic xxx. If xxx is nothing, the subscription will be made acording to the list of topics in EEPROM (see sharedFunctions.c)\n";
            outputBuf+="[MQTT_SENDMSG xxx yyy] Sends the message xxx on the topic yyy\n";
            outputBuf+="-------- END HELP SECTION ---------\n";
        }

        // #############################################################################
        // ###                         RESET COMMAND                                 ###
        // #############################################################################
        else if (!strcmp(firstParam, "RST")){
            ESP.restart();
        }
        
        // #############################################################################
        // ###                          IP COMMAND                                   ###
        // #############################################################################
                else if (!strcmp(firstParam,"IP")) {
            outputBuf+="IP configuration: [";
                    
            // ---------------------- Extracts the four IP parameters --------------------------
            for (i=2;i<6;i++){
                // Copies the current parameter from the input pointer
                memcpy(currentParam, paramList+ (i*MAX_PARAM_SIZE), MAX_PARAM_SIZE);
                tempBuf+=currentParam;
                if (i<5){
                    tempBuf+= '.';
                }
            }
            outputBuf+=tempBuf;
            // ---------------------- Handles the three IP types ------------------------------
            memcpy(currentParam, paramList+ (1*MAX_PARAM_SIZE), MAX_PARAM_SIZE);
            outputBuf+="] On parameter: [";
            outputBuf+=currentParam;
            outputBuf+=" ";
            outputBuf+=tempBuf;

            if (!strcmp(currentParam,"LOCAL")){
                setEEPROMParam(e_IP_Local,tempBuf);
                execStatus = true;
            }
            else if (!strcmp(currentParam,"GATEWAY")){
                setEEPROMParam(e_IP_Gateway,tempBuf);
                execStatus = true;
            }
            else if (!strcmp(currentParam,"SUBNET")){
                setEEPROMParam(e_IP_Subnet,tempBuf);
                execStatus = true;
            }
            outputBuf+="] ";

            // ------------------------------- User output --------------------------------------
            if (execStatus == true){
                outputBuf+=" Written successfully!\n";
            }
            else {
                outputBuf+=" Failed, check the parameters or run HELP.";
            }
        }

        // #############################################################################
        // ###                           RELAY COMMAND                               ###
        // #############################################################################

        else if (!strcmp(firstParam,"RELAY")){

            outputBuf+="Relay state: [";
            // ---------------------------- Handling relay state ------------------------------
            memcpy(currentParam, paramList+ (1*MAX_PARAM_SIZE), MAX_PARAM_SIZE);
            outputBuf+=currentParam;
            if (!strcmp(currentParam,"ON")) {
                setRelayState(HIGH);
                execStatus = true;
            }
            else if (!strcmp(currentParam,"OFF")){
                setRelayState(LOW);
                execStatus = true;
            }
            else {
                execStatus = false;
            }
            outputBuf+="] ";

            // ------------------------------- User output --------------------------------------
            if (execStatus == true){
                outputBuf+=" Changed successfully!\n";
            }
            else {
                outputBuf+=" Failed, check the parameters or run HELP.";
            }
        }

        // #############################################################################
        // ###                     MEMORY DISPLAY COMMAND                            ###
        // #############################################################################
        else if (!strcmp(firstParam,"MD")){
            // Displays the content of an EEPROM parameter
            
            // Retreives the parameter ID
            memcpy(currentParam, paramList+ (1*MAX_PARAM_SIZE), MAX_PARAM_SIZE);
            j = atof(currentParam);

            // User output and reading
            outputBuf+=("retreived parameter: ");
            outputBuf+=(getEEPROMParam(j));

        }
        // #############################################################################
        // ###                      MEMORY MODIFY COMMAND                            ###
        // #############################################################################
        else if (!strcmp(firstParam,"MM")){
            // Modifies the content of an EEPROM parameter

            // Retreives the two parameters
            memcpy(currentParam, paramList+ (1*MAX_PARAM_SIZE), MAX_PARAM_SIZE);
            i = atof(currentParam);
            memcpy(currentParam, paramList+ (2*MAX_PARAM_SIZE), MAX_PARAM_SIZE);

            // User output and writing
            outputBuf+="Modifying address[";
            outputBuf+=i;
            outputBuf+="]\n";
            j = setEEPROMParam(i,currentParam); // Writing
            if (j==0){
                outputBuf+="\nDONE\n";
            }
            else{
                outputBuf+="Incorrect input string, maximum size: ";
                outputBuf+=j;
            }
        }
        
        // #############################################################################
        // ###                        WIFI INIT COMMAND                              ###
        // #############################################################################
        else if (!strcmp(firstParam,"WIFI_INIT")){
            outputBuf+="Starting WiFi server...\n";
            InitWifiWsMode(0);
            outputBuf+="Wifi Started!\n";
        }
        
        // #############################################################################
        // ###                        MQTT INIT COMMAND                              ###
        // #############################################################################
        else if (!strcmp(firstParam,"MQTT_INIT")){
            outputBuf+="Starting MQTT server...\n";
            if (MQTT_Connect() == 0) {
                outputBuf+="MQTT server Started!\n";
            }
            else {
                outputBuf+="Fatal error on MQTT Server start!";
            }
        }

        // #############################################################################
        // ###                     MQTT SUBSCRIBRE COMMAND                           ###
        // #############################################################################
        else if (!strcmp(firstParam,"MQTT_SUB")){
            memcpy(currentParam, paramList+ (1*MAX_PARAM_SIZE), MAX_PARAM_SIZE);
            if (strcmp(currentParam,"")){
                MQTT_subscribeTopicsList();
            }
            else {
                MQTT_subscribe(currentParam);
            }
        }

        else if (!strcmp(firstParam,"MQTT_SENDMSG")){
            memcpy(firstParam, paramList+ (1*MAX_PARAM_SIZE), MAX_PARAM_SIZE);
            memcpy(currentParam, paramList+ (2*MAX_PARAM_SIZE), MAX_PARAM_SIZE);
            MQTT_sendMessage(firstParam, currentParam);
        }
    
        // #############################################################################
        // ###                          UNKNOWN COMMAND                              ###
        // #############################################################################

        else {
            outputBuf+="Unkown command, try HELP";
        }
        // Prints the output of execution
        addToSrl(outputBuf);
        // Empties the parameter lisr
        (*paramList) = 0;
    }
}