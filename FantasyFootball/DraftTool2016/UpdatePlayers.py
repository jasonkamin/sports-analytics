#!/usr/bin/env python 

# Created Apr 1.
# Frontend for UpdateFantasyProjections.C
#
# run './UpdatePlayers.py --help' for options.
#
# -Jason Kamin 1.Apr.2017

import os
import sys

_j_args = sys.argv

_playerdrafted = "N_U_L_L";
_team = 0;
_bail_out = 0;
_reset = 0;
_rollback = "N_U_L_L";

for i in range(0,len(_j_args)):
    if _j_args[i] == "-p":
        _playerdrafted = _j_args[i+1]
    if _j_args[i] == "-t":
        _team = _j_args[i+1]
    if _j_args[i] == "-r":
        if len(_j_args) > i+1:
            _rollback = _j_args[i+1]
        else:
            _rollback = "L_A_S_T"
    if _j_args[i] == "--help":
        _bail_out = 9
    if _j_args[i] == "--RESET":
        _reset = 1

print _reset;
if _reset==1:
    print "reseting everything!!"
    os.system("rm DraftedList.txt")
    os.system("rm DraftedTeam_1.txt")
    os.system("$ROOTSYS/$ROOT_SUBDIR/bin/root -l -q UpdateFantasyProjections.C")
    if os.path.isfile("DraftedList.txt") == 0:
        file_DraftedPlayerListNEW = open("DraftedList.txt",'w')
        file_DraftedPlayerListNEW.write("D.Johnson\n")
        file_DraftedPlayerListNEW.write("J.Jones\n")
        file_DraftedPlayerListNEW.close()
    if os.path.isfile("DraftedTeam_1.txt") == 0:
        file_DraftedPlayerListNEW = open("DraftedTeam_1.txt",'w')
        file_DraftedPlayerListNEW.close()
    sys.exit(0)


if _bail_out == 9:
    print "\n####################################################################"
    print   "____UpdatePlayers.py - A frontend for UpdateFantasyProjections.C____\n"
    print "./UpdatePlayers.py -p <A.Brown> -t 1"
    print "    -p : player name (L.Bell, A.Peterson, B.Roth).  this can be a substring."
    print "    -t : drafting team name (1,2,3...)  just an integer for now."
    print "    -r : rollback a drafted player (L.Bell, A.Peterson, B.Roth).  can be a substring."
    print "         default (no argument passed) is last player drafted. \n"
    print "    --reset : Reset the draft.  careful !"
    print "... quitting... \n"
    sys.exit(0)

if _playerdrafted != "N_U_L_L" and _rollback != "N_U_L_L":
    print " ** you can't DRAFT and ROLLBACK at the same time!! **"
    sys.exit(0)

if os.path.isfile("DraftedList.txt") == 0:
    file_DraftedPlayerListNEW = open("DraftedList.txt",'w')
    file_DraftedPlayerListNEW.close()
if os.path.isfile("DraftedTeam_1.txt") == 0:
    file_DraftedPlayerListNEW = open("DraftedTeam_1.txt",'w')
    file_DraftedPlayerListNEW.close()
    

if _playerdrafted == "N_U_L_L" and _rollback == "N_U_L_L":
    os.system("$ROOTSYS/$ROOT_SUBDIR/bin/root -l -q UpdateFantasyProjections.C")
    sys.exit(0)


#if len(_j_args) > 4:
#    _bail_out = 1
#
#if _bail_out == 1:
#    print "\nHand me less arguments!"
#    print "eg. like this: \n"
#    print "./UpdatePlayers.py -p <A.Brown> -t 1"
#    print "    -p : player name (L.Bell, A.Peterson, B.Roth).  this can be a substring.\n"
#    print "    -t : drafting team name (1,2,3...)  just an integer for now.\n"
#    print "    -r : rollback a drafted player (L.Bell, A.Peterson, B.Roth).  can be a substring."
#    print "         default (no argument passed) is last player drafted. \n"
#    print "... quitting... \n"
#    sys.exit(0)

quitstring = "quitting...";

playermatches = [];

########################################################
############# rollback a player ########################
if (_rollback == "L_A_S_T") or ("-" in _rollback):
    file_DraftedPlayerListNEW = open("DraftedList.txt.temp",'w')
    file_AlreadyDrafted       = open("DraftedList.txt",     'r')
    listofdraftedplayers = [];
    for line in file_AlreadyDrafted:
        listofdraftedplayers.append(line)
    for i in range(0,len(listofdraftedplayers)-1):
        file_DraftedPlayerListNEW.write(listofdraftedplayers[i])
    print "removing ", listofdraftedplayers[len(listofdraftedplayers)-1]
    file_DraftedPlayerListNEW.close()
    os.system("mv DraftedList.txt.temp DraftedList.txt")
    os.system("$ROOTSYS/$ROOT_SUBDIR/bin/root -l -q UpdateFantasyProjections.C")
    sys.exit(0)

elif _rollback != "N_U_L_L":
    file_AlreadyDrafted = open("DraftedList.txt",'r')
    for line in file_AlreadyDrafted:
        if _rollback in line:
            playermatches.append(line)

    if len(playermatches) > 1:
        print "more than one match for player ", _playerdrafted
        for i in range(0,len(playermatches)):
            print playermatches[i]
        sys.exit(0)
    elif len(playermatches) == 0:
        print "no drafted player matches that string!"
        print "... quitting..."
        sys.exit(0)
    else:
        file_DraftedPlayerListNEW = open("DraftedList.txt.temp",'w')
        file_AlreadyDrafted       = open("DraftedList.txt",     'r')
        for line in file_AlreadyDrafted:
            if _rollback in line:
                print "removing ", line
            else:
                file_DraftedPlayerListNEW.write(line)
        file_DraftedPlayerListNEW.close()
        os.system("mv DraftedList.txt.temp DraftedList.txt;")
        os.system("$ROOTSYS/$ROOT_SUBDIR/bin/root -l -q UpdateFantasyProjections.C")
        sys.exit(0)
############# rollback a player ########################
########################################################



########################################################
########### check if already drafted ###################
file_AlreadyDrafted = open("DraftedList.txt",'r')
for line in file_AlreadyDrafted:
    if _playerdrafted in line:
        playermatches.append(line)

if(len(playermatches)==1):
    print _playerdrafted, "  was already drafted ! (", playermatches[0], ")."
    sys.exit(0)
elif(len(playermatches)>1):
    print "multiple players matching ", _playerdrafted, "  were already drafted !"
    for i in range(0,len(playermatches)):
        print "  ", playermatches[i]
    sys.exit(0)
########### check if already drafted ###################
########################################################



########################################################
################ look up player ########################
file_TotalPlayerList = open("TotalList.txt",'r')
for line in file_TotalPlayerList:
    if _playerdrafted in line:
        #print _playerdrafted,"  --> ",line
        playermatches.append(line)

print "\n"

if(len(playermatches)>1):
    print "more than one match for player ", _playerdrafted
    for i in range(0,len(playermatches)):
        print playermatches[i]
    sys.exit(0)
################ look up player ########################
########################################################



########################################################
############# put on my team    ########################
file_DraftedPlayerList = open("DraftedList.txt",'a')
file_DraftedPlayerList.write(playermatches[0])
file_DraftedPlayerList.close()
print "Sucessfully drafted ",playermatches[0]

if _team > 0:
    text_teamDrafted = "DraftedTeam_" + str(_team) + ".txt"
    file_teamDrafted = open(text_teamDrafted,'a')
    file_teamDrafted.write(playermatches[0])
    file_teamDrafted.close()
#############  put on my team   ########################
########################################################



os.system("$ROOTSYS/$ROOT_SUBDIR/bin/root -l -q UpdateFantasyProjections.C")

sys.exit(0)


