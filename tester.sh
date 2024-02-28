#!/bin/bash

CYAN='\033[0;36m'
ORAN='\033[0;33m'
NCB='\033[1m'
NC='\033[0m'  # No Color

## Create input files
echo "Blue\nRed\nGray\nBlue\nTeal\nYellow\nPurple\nOrange\nPink\nBrown\nWhite\nBlack\nCyan\nMagenta\nTurquoise\nLavender\nIndigo\nMaroon\nBeige\nOlive\nCoral\nPeach\nGold\nSilver\nNavy\nViolet\nRuby\nEmerald\nSapphire\nAmber\nCharcoal\nIvory\nMauve\nFuchsia\nMint\nSlate\nTangerine\nPeriwinkle\nAsh\nLilac\nNavy Blue\nRed\nGray\nGreen\nTeal\nYellow\nPurple\nOrange\nPink\nBrown\nWhite\nBlack\nCyan\nMagenta\nTurquoise\nLavender\nIndigo\nMaroon\nBeige\nOlive\nCoral\nPeach\nGold\nSilver\nNavy\nViolet\nRuby\nEmerald\nSapphire\nAmber\nCharcoal\nIvory\nMauve\nFuchsia\nMint\nSlate\nTangerine\nPeriwinkle\nAsh\nLilac\nNavy Blue\nRed\nGray\nGreen\nTeal\nYellow\nPurple\nOrange\nPink\nBrown\nWhite\nBlack\nCyan\nMagenta\nTurquoise\nLavender\nIndigo\nMaroon\nBeige\nOlive\nCoral\nPeach\nGold\nSilver\nNavy\nViolet\nRuby\nEmerald\nSapphire\nAmber\nCharcoal\nIvory\nMauve\nFuchsia\nMint\nSlate\nTangerine\nPeriwinkle\nBurgundy\nLilac\nNavy Blue\nRed\nGray\nGreen\nTeal\nYellow\n" > infile
echo "This is not meant to be read" > noread_infile
chmod 111 noread_infile

PIPEX="./pipex"
IN="infile"
OUT="outfile"

Tests:
- too few arguments, too many arguments
- no infile, infile no read-permission (infile is a folder?)
- command 1, 2 or both dont exist
- command given as whole path
- use of single and double quotes within a command

Things to check
- outut file
- exit code

test=("

parameters1=("./pipex" "${IN}" "wc -l" "cat" "outfile1.txt")
parameters2=("./pipex" "${IN}" "grep a" "sed s/a/A/g" "outfile2.txt")
parameters3=("./pipex" "${IN}" "ls -l" "cat" "grep 1" "cat")
parameters4=("./pipex" "${IN}" "ls -l")
parameters5=("./pipex" "${IN}" "/bin/cat" "wc -w" "outfile5.txt")
parameters6=("./pipex" "poopoo" "cat" "wc -l" "outfile6.txt")
parameters7=("./pipex" "${IN}" "poopoo" "wc -l" "outfile7.txt")
parameters8=("./pipex" "${IN}" "cat" "poopdypoo" "outfile8.txt")
parameters9=("./pipex" "${IN}" "cat" "wc -w" "outfile9.txt")
parameters10=("./pipex" "${IN}" "" "" "outfile10.txt")

perform_test()
{
	echo -e "${ORAN}------RUNNING-TESTS------${NC}"
	echo -e "${CYAN}test 1 with parameters: ${parameters1[@]}${NC}"
	"${parameters1[@]}"
	echo -e "${CYAN}test 2 with parameters: ${parameters2[@]}${NC}"
	"${parameters2[@]}"
	echo -e "${CYAN}test 3 with too many parameters${NC}"
	"${parameters3[@]}"
	echo -e "${CYAN}test 4 with too few parameters${NC}"
	"${parameters4[@]}"
	echo -e "${CYAN}test 5: full address of command: ${parameters5[@]}${NC}"
	"${parameters5[@]}"
	echo -e "${CYAN}test 6 with invalid first argument (infile)${NC}"
	"${parameters6[@]}"
	echo -e "${CYAN}test 7 with invalid second argument (1st command)${NC}"
	"${parameters7[@]}"
	echo -e "${CYAN}test 8 with invalid third argument (2nd command)${NC}"
	"${parameters8[@]}"
	touch outfile9.txt && chmod 0111 outfile9.txt
	echo -e "${CYAN}test 9 with outfile open error${NC}"
	"${parameters9[@]}"
	echo -e "${CYAN}test 10 with an empty string as a command argument${NC}"
	"${parameters10[@]}"
	echo -e "${ORAN}------CHECKING-OUTPUT-FILES------${NC}"
	echo -e "${CYAN}outfile1.txt:${NC}" && cat outfile1.txt
	echo -e "${CYAN}outfile2.txt:${NC}" && cat outfile2.txt
	echo -e "${CYAN}outfile5.txt:${NC}" && cat outfile5.txt
	echo -e "${CYAN}outfile6.txt:${NC}" && cat outfile6.txt
	echo -e "${CYAN}outfile7.txt:${NC}" && cat outfile7.txt
	echo -e "${CYAN}outfile8.txt:${NC}" && cat outfile8.txt
	echo -e "${CYAN}outfile9.txt:${NC}" && chmod 0644 outfile9.txt && cat outfile9.txt
	echo -e "${CYAN}outfile10.txt:${NC}" && cat outfile10.txt
	echo -e "${ORAN}------END-OF-TEST------${NC}"
}

remove_files()
{
	echo "Removing test output files"
	rm outfile1.txt
	rm outfile2.txt
	rm outfile5.txt
	rm outfile6.txt
	rm outfile7.txt
	rm outfile8.txt
	chmod 0777 outfile9.txt && rm outfile9.txt
	rm outfile10.txt
	echo "Removing test intput file"
	rm infile
}

if [ "$1" == "remove" ]; then
	remove_files
else
	perform_test
fi