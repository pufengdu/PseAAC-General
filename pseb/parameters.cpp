/****************************************************************************
 * parameter.cpp - Copyright 2013 Pufeng Du, Ph.D.                          *
 *                                                                          *
 * This file is part of PseAAC-Builder v3.0.                                *
 * Pseaac-Builder is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * PseAAC-Builder is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with PseAAC-Builder.  If not, see <http://www.gnu.org/licenses/>.  *
 ****************************************************************************/

#include "parameters.h"

error_t paraOptionParser (int key, char *arg, struct argp_state *state)
{
    parameters *para = (parameters*)(state->input);
    switch (key)
    {
        case 'q':
            para->queryflag = 1;
            break;
        case 'i':
            para->inputfile = arg;
            break;
        case 'o':
            para->outputfile = arg;
            break;
        case 'a':
            para->useaac = true;
            break;
        case 'd':
            para->usedip = true;
            break;
        case 'b':
            para->bemfile = arg;
            break;
        case 'e':
            para->luafile = arg;
            break;
        case 't':
        {
            para->usepse = true;
            char *tmpType = arg;
            int tmpPseType = atoi(tmpType);
            if (tmpPseType == 0)
            {
                para->pse = PSE_CLASSIC;
            }
            else if (tmpPseType == 1)
            {
                para->pse = PSE_AMPHIPHILIC;
            }
            else if (tmpPseType == 2)
            {
                para->pse = PSE_GENERAL_BEMS;
                para->WriteOutFile = ofWriteBinary;
            }
            else if (tmpPseType == 3)
            {
                para->pse = PSE_GENERAL_USERS;
            }
            else
            {
                emProcess(EK_ABORT_PARAMETER_ERROR,tmpType,"-t type code out of range");
            }
        }
            break;
        case 'w':
        {
            para->with_w = true;
            para->w = strtof(arg,NULL);
        }
            break;
        case 'l':
            para->l = atoi(arg);
            break;
        case 'x':
            para->pcselfile = arg;
            break;
        case 'n':
            para->user_settings = arg;
            break;
        case 'm':
            if (strcasecmp(arg,"csv") == 0)
            {
                para->outfmt = OPF_CSV;
                break;
            }
            else if (strcasecmp(arg,"svm")==0)
            {
                para->outfmt = OPF_SVM;
                break;
            }
            else if (strcasecmp(arg,"tab")==0)
            {
                para->outfmt = OPF_MAT;
                break;
            }
            else
            {
                emProcess(EK_ABORT_PARAMETER_ERROR, arg, "-m option can only be chosen from svm, tab and csv");
            }
            break;
        case 's':
        {
            int tmpSeg = atoi(arg);
            if (tmpSeg < 1 || tmpSeg > 4)
            {
                emProcess(EK_ABORT_PARAMETER_ERROR, arg, "-s option can only be an integer in [1,4]");
            }
            para->cntseg=tmpSeg;
        }
        break;
        case ARGP_KEY_ARG:
            if (state->arg_num > 0)
            {
                argp_usage(state);
            }
            break;
        case ARGP_KEY_END:
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

parameters *paraParseCmdLine(int argc, char* argv[], struct pcprop *proplib)
{
    /*Parsing command-line with GNU Arg Parser*/
    /*Refer to http://crasseux.com/books/ctutorial/argp-example.html#argp%20example*/

    struct argp_option options[] =
    {
        {"query", 'q',0,0, "Since pseaac includes all AAIndex database inside the program, this option is used to export a list of physicochemical properties names for your choice. This option is an immediate option. When this option appears on the command line, all other options will be ignored. When this option appears with the other immediate option, the function of the first option (from left to right) will be executed."},
        {"input",'i',"INPUT_FILE",0,"INPUT_FILE should be a valid fasta formated file. The additional restriction is in the annotation lines, which was always started by \">\". Every annotation line in the input fasta file must contain two and only two fields, which are seperated by \"|\". The first field is the id of the sequences, which can be gi numbers, uniprot id, accession numbers or anything t hat can identify the sequence uniquely in the fasta file. The second field is an integer, which is recognized as the class label when you export the result. If you do not need a class label, simply put a \"0\" in this field. If you do not specify the input_file_name, please omit the -i option completely. In this case, the STDIN will be used as the  input file."},
        {"output",'o',"OUTPUT_FILE",0,"OUTPUT_FILE can be any valid file that can be written with your own privilige on your system. The program will overwrite the file if it has already existed. If you do not specify the OUTPUT_FILE, please omit the this option completely. In this case, the STDOUT will be used as the output file."},
        {"select-aaindex",'x',"SELECTION_FILE",0,"SELECTION_FILE is a text file that contains the names of selected physicochemical properties. Every line of this file contains the name of one physicochemical property. These names should be from the list that can be produced by the -q option. The physicochemical properties that are listed in this file will be used for calculating the pseudo-amino acid compositions. If this option is ommited, the choice of the physicochemical properties will be decided by the -t option. The physicochemical properties of original form of pseudo-amino acid composition will be used."},
        {"output-format",'m', "{svm|csv|tab}",0,"The output format can be selected from the following three. (1) svm: The libSVM training data format. (2) csv: The format that can be loaded into spreadsheet softwares. (3) tab: Simple format delimited by TAB. If this option is ommited, the tab format will be default."},
        {"segment", 's',"SEG", 0, "SEG should be an integer between 1 and 4. The sequence will be segmented into SEG segments before constructing pseudo-amino acid compositions. The feature vector will be simple concated vector of each segments. The default value of this option is 1."},
        {"aminoacid",'a',0,0,"This option is used to indicate the amino acid compositions should be included in the model. If this option is ommited, the amino acid compositions will not be included. If the -d option is ommited also, the -w option will also be ignored."},
        {"dipeptide",'d',0,0,"This option is used to indicate the dipeptide composition should be included in the model. If this option is ommited, the dipeptide composition will not be included. If the -a option is ommited also, the -w option will be ignored."},
        {"type",'t',"PSE_TYPE",0,"PSE_TYPE can be 0, 1, 2 or 3. \n0 - classic pseudo-amino acid composition. \n1 - amphiphilic pseudo-amino acid composition. \n2 - the BEMs based features. \n3 - User defined Lua scripts. \nIf this option is ommited, the -w option will be ignored. \nIf this option is set to 2, -a -l -x and -w options will be ignored and the -b option become mandatory."},
        {"weight",'w',"WEIGHT",0, "The WEIGHT parameter in the pseudo-amino acid composition algorithm can be a value between (0,1]. If you do not specify the WEIGHT parameter, the program will not use w in the algorithm to balance the pseudo-factors and the real-factors. If the -d and the -a options are both used, the WEIGHT will be applied to balaced all first 420 dimensions of the feature vector. When -s is used, w will be used for every segments seperately." },
        {"lambda",'l',"LAMBDA",0,"The LAMBDA parameter in the pseudo-amino acid composition algorithm can be any integer in the range (0,20]. The default value for LAMBDA is 10. When -s is used, LAMBDA will be used for every segments seperately. Please bear in mind that LAMBDA must be less than the length of every segment. This is the same restriction as the PseAAC server. However, PseAAC-Builder would not complain the invalid value of LAMBDA. But the result for too big LAMBDA is completely meaningless." },
        {"bem-file", 'b', "BEM_FILE",0,"BEM_FILE can be any valid BEM formate file that was provided with the pseaac-builder, different BEM_FILE will generate different types of general form pseaac. If -t option was not set to 2, this option will be ignored"},
        {"user-ext",'e',"LUA_FILE",0,"LUA_FILE is a Lua script that follows the rules of PSEB User Extension rules."},
        {"ext-options",'n',"VAR_STRING",0,"VAR_STRING is a set of lua variables like name1=value1;name2=value2;... that are seperated by semicolon, these variables will be executed only once at the begining of the script"},

        {0}
    };
    /*
        GNU Arg Parser standard version declaration
    */
    argp_program_version = "PseAAC-Builder version 3.0";
    argp_program_bug_address = "Dr. Pufeng Du. <PufengDu@gmail.com>";
    char args_doc[] = "";
    char doc[] = "PseAAC-Builder -- A program for computing pseudo-amino acid compositions from protein sequences.\vReleased by \n---\nPufeng Du. Ph.D. \nSchool of Computer Science and Technology, Tianjin University\nTianjin 300072, China\n";

    struct argp argp = {options, paraOptionParser, args_doc, doc};
    parameters *mypara = (parameters*)calloc(sizeof(parameters),1);
    mypara->cntseg = 1;
    mypara->l = 10;
    mypara->w = 0.05;
    mypara->with_w = false;
    mypara->WriteOutFile = ofWriteDecimal;
    argp_parse(&argp,argc,argv,0,0,mypara);
    return mypara;
}

