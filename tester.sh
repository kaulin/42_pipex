#!/bin/bash

# Define colors
CYAN='\033[0;36m'
OLIVE='\033[0;33m'
NCB='\033[1m' # Bold
NC='\033[0m'  # No Color

# Define keywords
PIP="./pipex"
IN="meow/infile"
OUT="meow/outfile"
NOR="meow/noread"
NOW="meow/nowrite"
NULL=""

# Declare test counter
declare -i COUNTER=1

# Welcome banner function, text made with https://patorjk.com/software/taag/
function welcome() {
echo -e "${NCB}
 ____  __  ____  ____  _  _    ____  ____  ____  ____  ____  ____ 
(  _ \\(  )(  _ \\(  __)( \\/ )  (_  _)(  __)/ ___)(_  _)(  __)(  _ \\
 ) __/ )(  ) __/ ) _)  )  (     )(   ) _) \\___ \\  )(   ) _)  )   /
(__)  (__)(__)  (____)(_/\\_)   (__) (____)(____/ (__) (____)(__\\_)
${OLIVE}- By Jaakko Junttila (jajuntti@student.hive.fi)${NC}\n"
}

# Initialization function
function init() {
	echo "Creating test files"
	mkdir meow
	echo "Blue\nRed\nGray\nBlue\nTeal\nYellow\nPurple\nOrange\nPink\nBrown\nWhite\nBlack\nCyan\nMagenta\nTurquoise\nLavender\nIndigo\nMaroon\nBeige\nOlive\nCoral\nPeach\nGold\nSilver\nNavy\nViolet\nRuby\nEmerald\nSapphire\nAmber\nCharcoal\nIvory\nMauve\nFuchsia\nMint\nSlate\nTangerine\nPeriwinkle\nAsh\nLilac\nNavy Blue\nRed\nGray\nGreen\nTeal\nYellow\nPurple\nOrange\nPink\nBrown\nWhite\nBlack\nCyan\nMagenta\nTurquoise\nLavender\nIndigo\nMaroon\nBeige\nOlive\nCoral\nPeach\nGold\nSilver\nNavy\nViolet\nRuby\nEmerald\nSapphire\nAmber\nCharcoal\nIvory\nMauve\nFuchsia\nMint\nSlate\nTangerine\nPeriwinkle\nAsh\nLilac\nNavy Blue\nRed\nGray\nGreen\nTeal\nYellow\nPurple\nOrange\nPink\nBrown\nWhite\nBlack\nCyan\nMagenta\nTurquoise\nLavender\nIndigo\nMaroon\nBeige\nOlive\nCoral\nPeach\nGold\nSilver\nNavy\nViolet\nRuby\nEmerald\nSapphire\nAmber\nCharcoal\nIvory\nMauve\nFuchsia\nMint\nSlate\nTangerine\nPeriwinkle\nBurgundy\nLilac\nNavy Blue\nRed\nGray\nGreen\nTeal\nYellow" > meow/noread
	sed 's/\\n/\'$'\n''/g' meow/noread | cat > meow/infile
	chmod 111 meow/noread
	echo "This is not meant to be written in" > meow/nowrite_pipex
	cp meow/nowrite_pipex meow/nowrite_shell
	chmod 111 meow/nowrite_pipex meow/nowrite_shell
	echo "This is meant to be overwritten" > meow/outfile1_pipex
	echo "This is meant to be overwritten" > meow/outfile1_shell
	echo -e "${CYAN}Done!${NC}"
}

# Norm checker function
function norm() {
	local norm_result

	while true; do
		echo ""
		echo -en "${NCB}Check norm with: norminette -R CheckForbiddenSourceHeader? (y/n): ${NC}" 
		read answer
		case $answer in
			[Yy]* )	
				norm_result=$(norminette -R CheckForbiddenSourceHeader | grep -v OK!)
				if [[ -z $norm_result ]]
				then
					echo -e "${CYAN}Done! No norm errors!${NC}"
				else
					echo -e "${norm_result}"
					echo -e "${OLIVE}Norm errors!${NC}"
				fi
				proceed
				break;;
			[Nn]* )
				echo -e "${OLIVE}Norm check skipped! Proceeding with tests.${NC}"
				break;;
			* ) echo -e "${OLIVE}Please enter 'y' for yes or 'n' for no.${NC}";;
		esac
	done
}

# Test arguments function
function test_args() {
	echo ""
	echo -e "${NCB}Test 0: ${NC}Number of arguments"
	echo -e "Error for too few arguments:"
	eval ./pipex meow/infile 'sort' meow/outfile
	echo ""
	echo -e "Error for too many arguments:"
	eval ./pipex meow/infile 'sort' 'uniq' 'wc -l' meow/outfile
	proceed
}

# Proceed function
function proceed() {
	while true; do
		echo ""
		echo -en "${NCB}Continue? (y/n): ${NC}" 
		read answer
		case $answer in
			[Yy]* ) break;;
			[Nn]* )
				clean;;
			* ) echo -e "${OLIVE}Please enter 'y' for yes or 'n' for no.${NC}";;
		esac
	done
}

# Test function
function test() {
	local desc=$1
	local inf=$2
	local c1=$3
	local c2=$4
	local outf=$5
	local p_outf=""
	local s_outf=""
	local p_cmd=""
	local s_cmd=""
	local comp=""

	if [ "$outf" = "${OUT}" ]
	then
		outf+="${COUNTER}"
	fi
	p_outf="${outf}_pipex"
	s_outf="${outf}_shell"
	p_cmd="${PIP} $inf '$c1' '$c2' $p_outf"
	s_cmd="< $inf $c1 | $c2 > $s_outf"

	echo ""
	echo -e "${NCB}Test ${COUNTER}: ${NC}$desc"
	echo -e "Pipex command: $p_cmd"
	echo -e "--- Pipex error messages: ---${OLIVE}"
	eval $p_cmd
	echo -e "${NC}-----------------------------"
	echo -e "Pipex exit status: $?"

	echo -e "Shell command: $s_cmd"
	echo -e "--- Shell error messages: ---${OLIVE}"
	eval $s_cmd
	echo -e "${NC}-----------------------------"
	echo -e "Shell exit status: $?"

	comp=$(diff "$p_outf" "$s_outf")
	if [ -z "$comp" ]
	then
		echo -e "${CYAN}Outfiles match!${NC}"
	else
		echo -e "${OLIVE}Outfiles differ:${NC}"
		cat -e "${comp}"
		cat_outs
	fi
	COUNTER+=1
	proceed
}

# Cat outfiles function
function cat_outs() {
	while true; do
		echo ""
		echo -en "${NCB}Print outfiles? (y/n): ${NC}" 
		read answer
		case $answer in
			[Yy]* ) 
				echo -e "${OLIVE}Pipex output:${NC}"
				cat "${p_outf}"
				echo -e "${CYAN}Shell output:${NC}"
				cat "${s_outf}";;
			[Nn]* )
				break;;
			* ) echo -e "${OLIVE}Please enter 'y' for yes or 'n' for no.${NC}";;
		esac
	done
}

# Clean function
function clean() {
	while true; do
		echo ""
		echo -en "${NCB}Do you wish to remove the created files? (y/n): ${NC}" 
		read answer
		case $answer in
			[Yy]* )
				echo -e "Deleting test files and exiting ${NCB}meow!${NC}"
				 rm -rf meow
				exit 0;;
			[Nn]* )
				echo -e "Exiting ${NCB}meow!${NC} Test files located in ${NCB}meow!${NC}"
				chmod 755 ${NOR} ${NOW}
				exit 0;;
			* ) echo -e "${OLIVE}Please enter 'y' for yes or 'n' for no.${NC}";;
		esac
	done
}

# Call with remove argument to clear previously created test files
if [ "$1" == "remove" ]
then
	rm -rf meow
	exit 0;
fi

# Preliminaries
welcome
init
norm
test_args

# INSERT TESTS BELOW (FORMAT: "DESCRIPTION" "INFILE" "CMD1" "CMD2" "OUTFILE")
test "Simple test, existing outfile should be overwritten"  "${IN}" "sort" "uniq" "${OUT}"
# Infile & Outfile tests
test "Infile does not exist" "bad_cat" "sort" "ls" "${OUT}"
test "Infile is empty string" $NULL "sort" "ls" "${OUT}"
test "No read permission for infile" "${NOR}" "sort" "ls" "${OUT}"
test "No write permission for outfile" "${IN}" "sort" "ls" "${NOW}"
# Command tests
test "First command is empty string" "${IN}" $NULL "ls" "${OUT}"
test "Second command is empty string" "${IN}" "ls" $NULL "${OUT}"
test "First command does not exist" "${IN}" "bad_cat" "ls" "${OUT}"
test "Second command does not exist" "${IN}" "ls" "bad_cat" "${OUT}"
test "Command handles quotes" "${IN}" "grep \" Blue\"" "wc -l" "${OUT}"

# INSERT TESTS ABOVE

# Finish up
echo -e "${CYAN}Done! You've completed all $((${COUNTER}-1)) tests.${NC}"
clean

# Test cases TODO
# too few arguments, too many arguments
# no infile, infile no read-permission (infile is a folder?)
# command 1, 2 or both dont exist, or is null
# command given as whole path
# use of single and double quotes within a command