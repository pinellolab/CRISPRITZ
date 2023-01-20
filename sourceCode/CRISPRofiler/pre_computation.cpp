#include "include/crispritz.h"

using namespace std;

extern int genlen, pamlimit, guidelen, threads, inputmissmatch;
extern vector<string> guides;
extern string genome;
vector<vector<int>> guideprofiling;					   // vector profile
vector<vector<vector<vector<int>>>> matrixprofiling;   // vector extended profile
vector<bitset<4>> genomebit;						   // genome in bit
vector<vector<bitset<4>>> guidesbit, reverseguidesbit; // guides and reverseguides in bit
extern bool pam_at_start;

void genomebitconversion() // converto il genoma dal fasta alla versione bit
{
	genomebit.clear();
	genomebit.resize(genlen);

#pragma omp parallel for num_threads(threads) schedule(static)
	for (int i = 0; i < genlen; i++)
	{
		if (genome[i] == 'A')
		{
			genomebit[i] = bitset<4>(string("0001"));
		}
		else if (genome[i] == 'C')
		{
			genomebit[i] = bitset<4>(string("0010"));
		}
		else if (genome[i] == 'G')
		{
			genomebit[i] = bitset<4>(string("0100"));
		}
		else if (genome[i] == 'T')
		{
			genomebit[i] = bitset<4>(string("1000"));
		}
		else if (genome[i] == 'N')
		{
			genomebit[i] = bitset<4>(string("0000"));
		}
		else if (genome[i] == 'R')
		{
			genomebit[i] = bitset<4>(string("0101"));
		}
		else if (genome[i] == 'Y')
		{
			genomebit[i] = bitset<4>(string("1010"));
		}
		else if (genome[i] == 'S')
		{
			genomebit[i] = bitset<4>(string("0110"));
		}
		else if (genome[i] == 'W')
		{
			genomebit[i] = bitset<4>(string("1001"));
		}
		else if (genome[i] == 'K')
		{
			genomebit[i] = bitset<4>(string("1100"));
		}
		else if (genome[i] == 'M')
		{
			genomebit[i] = bitset<4>(string("0011"));
		}
		else if (genome[i] == 'B')
		{
			genomebit[i] = bitset<4>(string("1110"));
		}
		else if (genome[i] == 'D')
		{
			genomebit[i] = bitset<4>(string("1101"));
		}
		else if (genome[i] == 'H')
		{
			genomebit[i] = bitset<4>(string("1011"));
		}
		else if (genome[i] == 'V')
		{
			genomebit[i] = bitset<4>(string("0111"));
		}
	}
}

void guidesbitconversion() // converto le guide dal file.txt alla versione bit
{
	guidesbit.resize(guides.size());
	reverseguidesbit.resize(guides.size());

	// #pragma omp parallel for num_threads(threads) schedule(static)
	for (int i = 0; i < guides.size(); i++)
	{
		int guidelencorrected = guides[i].length(); // include PAM bps
		guidesbit[i].resize(guidelencorrected);
		reverseguidesbit[i].resize(guidelencorrected);

		// if (!pam_at_start) // pam at end 5'
		// {
		for (int j = 0; j < guidelencorrected; j++)
		{
			if (guides[i][j] == 'A')
			{
				guidesbit[i][j] = bitset<4>(string("0001"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1000"));
			}
			else if (guides[i][j] == 'C')
			{
				guidesbit[i][j] = bitset<4>(string("0010"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0100"));
			}
			else if (guides[i][j] == 'G')
			{

				guidesbit[i][j] = bitset<4>(string("0100"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0010"));
			}
			else if (guides[i][j] == 'T')
			{
				guidesbit[i][j] = bitset<4>(string("1000"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0001"));
			}
			else if (guides[i][j] == 'N')
			{
				guidesbit[i][j] = bitset<4>(string("1111"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1111"));
			}
			else if (guides[i][j] == 'R')
			{
				guidesbit[i][j] = bitset<4>(string("0101"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1010"));
			}
			else if (guides[i][j] == 'Y')
			{
				guidesbit[i][j] = bitset<4>(string("1010"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0101"));
			}
			else if (guides[i][j] == 'S')
			{
				guidesbit[i][j] = bitset<4>(string("0110"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1001"));
			}
			else if (guides[i][j] == 'W')
			{
				guidesbit[i][j] = bitset<4>(string("1001"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0110"));
			}
			else if (guides[i][j] == 'K')
			{
				guidesbit[i][j] = bitset<4>(string("1100"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0011"));
			}
			else if (guides[i][j] == 'M')
			{
				guidesbit[i][j] = bitset<4>(string("0011"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1100"));
			}
			else if (guides[i][j] == 'B')
			{
				guidesbit[i][j] = bitset<4>(string("1110"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0111"));
			}
			else if (guides[i][j] == 'D')
			{
				guidesbit[i][j] = bitset<4>(string("1101"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1011"));
			}
			else if (guides[i][j] == 'H')
			{
				guidesbit[i][j] = bitset<4>(string("1011"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1101"));
			}
			else if (guides[i][j] == 'V')
			{
				guidesbit[i][j] = bitset<4>(string("0111"));
				reverseguidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1110"));
			}
		}
		// 	}
		// 	else
		// 	{
		// 		for (int j = 0; j < guidelencorrected; j++)
		// 		{
		// 			if (guides[i][j] == 'A')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("0001"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1000"));
		// 			}
		// 			else if (guides[i][j] == 'C')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("0010"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0100"));
		// 			}
		// 			else if (guides[i][j] == 'G')
		// 			{

		// 				reverseguidesbit[i][j] = bitset<4>(string("0100"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0010"));
		// 			}
		// 			else if (guides[i][j] == 'T')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("1000"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0001"));
		// 			}
		// 			else if (guides[i][j] == 'N')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("1111"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1111"));
		// 			}
		// 			else if (guides[i][j] == 'R')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("0101"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1010"));
		// 			}
		// 			else if (guides[i][j] == 'Y')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("1010"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0101"));
		// 			}
		// 			else if (guides[i][j] == 'S')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("0110"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1001"));
		// 			}
		// 			else if (guides[i][j] == 'W')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("1001"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0110"));
		// 			}
		// 			else if (guides[i][j] == 'K')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("1100"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0011"));
		// 			}
		// 			else if (guides[i][j] == 'M')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("0011"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1100"));
		// 			}
		// 			else if (guides[i][j] == 'B')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("1110"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("0111"));
		// 			}
		// 			else if (guides[i][j] == 'D')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("1101"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1011"));
		// 			}
		// 			else if (guides[i][j] == 'H')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("1011"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1101"));
		// 			}
		// 			else if (guides[i][j] == 'V')
		// 			{
		// 				reverseguidesbit[i][j] = bitset<4>(string("0111"));
		// 				guidesbit[i][guidelencorrected - 1 - j] = bitset<4>(string("1110"));
		// 			}
		// 		}
		// 	}
		// }
	}
}

void profilersetting() // alloco spazio per matrici dei profili
{
	guideprofiling.resize(guides.size());
	matrixprofiling.resize(guides.size());

	for (int i = 0; i < guides.size(); i++)
	{
		guideprofiling[i].resize(guidelen + 3 + inputmissmatch + 1);
		matrixprofiling[i].resize(inputmissmatch + 1);

		for (int kk = 0; kk <= inputmissmatch; kk++)
		{
			matrixprofiling[i][kk].resize(guidelen);

			for (int jj = 0; jj < guidelen; jj++)
			{
				matrixprofiling[i][kk][jj].resize(5);
			}
		}
	}
}
