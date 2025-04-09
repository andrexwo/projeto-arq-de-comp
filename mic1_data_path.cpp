#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <bitset>

using namespace std;

void ler_arquivo(string nomeArq, vector<string>& linhas) { // funcao para ler arquivo de entrada e armazenar as linhas de codigo em um vector
    string linha;
    ifstream arq(nomeArq);

    if (!arq.is_open()) {
        cerr << "erro ao abrir o arquivo!" << endl;
        exit(1);
    }

    while (getline(arq, linha)) {
        linhas.push_back(linha);
    }

    arq.close();
}



void str_to_int(string linha, vector<int>& inst) { // funcao com finalidade de pegar uma linha de codigo e adicionar cada valor em um vector de inteiros, estes que contem cada valor de entrada da ula
    for (char c : linha) {
        if (isdigit(c)) {
            inst.push_back(c - '0');
        }
    }   
}

typedef struct { // tipo instrucao, ela contem todas as entradas da ula
    int sll8, sra1;
    int f0, f1, enA, enB, invA, inc;
} Instruction;

void atribuir_instrucoes(Instruction& in, vector<int>& c) { // funcao para atribuir na variavel tipo instrucao os valores das entradas da ula, armazenadas no vector de inteiros
    in.sll8 = c[0];
    in.sra1 = c[1];
    in.f0 = c[2];
    in.f1 = c[3];
    in.enA = c[4];
    in.enB = c[5];
    in.invA = c[6];
    in.inc = c[7];
}

uint32_t somar_com_carry(uint32_t a, uint32_t b, int carry_in, int& carry_out) { // funcao de soma com carry in e out, passando o carryout com & para ele ser modificado fora da funcao, caso nao houvese o &, o valor de carry out so seria modificado dentor da funcao, o que nao seria util
    uint32_t resultado = 0;
    carry_out = carry_in;

    for (int i = 0; i < 32; i++) {
        int bitA = (a >> i) & 1;
        int bitB = (b >> i) & 1;

        int soma = bitA ^ bitB ^ carry_out;
        carry_out = (bitA & bitB) | (bitA & carry_out) | (bitB & carry_out);
        
        resultado |= (soma << i);
    }

    return resultado;
}

void ULA(uint32_t A, uint32_t B) { // funcao ula, aqui onde as operacoes serao feitas
    vector<string> linhas;
    string nomeArquivo = "C:\\Users\\andre\\Documents\\GitHub\\projeto-arq-de-comp\\etapa2_parte1.txt"; //strings contendo os enderecos dos arquivos para leitura e escrita, apenas mude para o que desejar

    ofstream arq_saida("C:\\Users\\andre\\Documents\\GitHub\\projeto-arq-de-comp\\saida2_parte1.txt");
    if (!arq_saida.is_open()) {
        cerr << "nao foi possivel abrir o arquivo" << endl;
    }

    uint32_t C = 0; // valor de saida, poderia ser S
    uint32_t Sd = C;

    int carry = 0;
    int Z = 0, N = 0;
    int cont = 0;

    ler_arquivo(nomeArquivo, linhas);

        arq_saida << "B....:" << bitset<32>(B) << endl; // demonstracao dos valores iniciais, antes de iniciar o programa
        arq_saida << "A....:" << bitset<32>(A) << endl;
        arq_saida << "Start of Program" << endl;
        arq_saida << "==============================================" << endl;

    for (size_t pc = 0; pc < linhas.size(); pc++) { // a variavel pc mostra em qual operacao estamos
        
        vector<int> inst;
        str_to_int(linhas[pc], inst);
        arq_saida << "IR = " << linhas[pc] << endl;


        Instruction i;
        atribuir_instrucoes(i, inst);

        //aqui onde as operacoes sao selecionadas, de acordo com os valores contidos na variavel tipo instrucao
        if(i.f0 == 0 && i.f1 == 0) { // operacoes logicas basicas
            if (i.enA == 1 && i.enB == 0 && i.invA == 0) {
                C = A;
                uint32_t aux;
                aux = B;
                B = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                } 
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                arq_saida << "b....: " << bitset<32>(B) << endl;
                arq_saida << "a....: " << bitset<32>(A) << endl;
                }
                B = aux;
            }
            else if (i.enA == 0 && i.enB == 1 && i.invA == 0) {
                C = B;
                uint32_t aux;
                aux = A;
                A = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                A = aux;
            }
            else if (i.enA == 1 && i.enB == 0 && i.invA == 1) {
                C = ~A;
                uint32_t aux;
                aux = B;
                B = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                B = aux;
            }
            else if(i.enA == 0 && i.enB == 1 && i.invA == 1) {
                C = ~B;
                uint32_t aux;
                aux = A;
                A = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                A = aux;
            }
            else if (i.enA == 1 && i.enB == 1 && i.invA == 0) {
                C = A & B;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
            } else if (i.enA == 0 && i.enB == 0 && i.invA == 0) {
                C = 0;
                uint32_t aux, aux2;
                aux = A;
                aux2 = B;
                A = B = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                A = aux;
                B = aux2;
            }
        }
        else if (i.f0 == 1 && i.f1 == 1) { //op. aritmeticas
            if (i.enA == 1 && i.enB == 1 && i.invA == 0 && i.inc == 0) {
                C = somar_com_carry(A, B, 0, carry);
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
            }
            else if (i.enA == 1 && i.enB == 1 && i.invA == 0 && i.inc == 1) {
                C = somar_com_carry(A, B, 1, carry);
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
            }
            else if (i.enA == 1 && i.enB == 0 && i.invA == 0 && i.inc == 1) {
                uint32_t x = 1;
                uint32_t aux;
                aux = B;
                B = 0;
                C = somar_com_carry(A, x, 0, carry);
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                B = aux;
            }
            else if (i.enA == 0 && i.enB == 1 && i.invA == 0 && i.inc == 1) {
                uint32_t x = 1;
                uint32_t aux;
                aux = A;
                A = 0;
                C = somar_com_carry (x, B, 0, carry);
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                A = aux;
            }
            else if (i.enA == 1 && i.enB == 1 && i.invA == 1 && i.inc == 1) {
                C = somar_com_carry(B, ~A, 1, carry);
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
            }
            else if (i.enA == 0 && i.enB == 1 && i.invA == 0 && i.inc == 0) {
                C = B - 1;
                uint32_t aux;
                aux = A;
                A = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                A = aux;
            }
            else if (i.enA == 1 && i.enB == 0 && i.invA == 1 && i.inc == 1) {
                C = -A;
                uint32_t aux;
                aux = B;
                B = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                B = aux;
            }
            else if (i.enA == 0 && i.enB == 0 && i.invA == 1 && i.inc == 0) {
                C = -1;
                uint32_t aux, aux2;
                A = aux;
                B = aux2;
                A = B = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                A = aux;
                B = aux2;
            }
        }
        else if (i.f0 == 0 && i.f1 == 1) {
            if (i.enA == 1 && i.enB == 1 && i.invA == 0 && i.inc == 0) {
                C = A | B;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
            }
        }
        else if (i.f0 == 1 && i.f1 == 0) {
            if (i.enA == 0 && i.enB == 0 && i.invA == 0 && i.inc == 1) {
                C = 1;
                uint32_t aux, aux2;
                A = aux;
                B = aux2;
                A = B = 0;
                if (i.sll8 == 1 && i.sra1 == 0) {
                    Sd = C << 8;
                } else if (i.sll8 == 0 && i.sra1 == 1) {
                    Sd = (int32_t)C >> 1;
                }
                Z = (Sd == 0);
                N = ((int32_t)Sd < 0);
                arq_saida << "Cycle: " << pc + 1 << endl << endl;
                arq_saida << "PC...: " << pc + 1 << endl;
                if (!(i.sll8 == 1 && i.sra1 == 1)) {
                    arq_saida << "b....: " << bitset<32>(B) << endl;
                    arq_saida << "a....: " << bitset<32>(A) << endl;
                    }
                A = aux;
                B = aux2;
                
            }
        }

       if (!(i.sll8 == 1 && i.sra1 == 1)) {
        arq_saida << "C....: " << bitset<32>(C) << endl;
        arq_saida << "Cd...: " << bitset<32>(Sd) << endl;
        arq_saida << "N....: " << N << endl;
        arq_saida << "Z....: " << Z << endl;
        arq_saida << "Co...: " << carry << endl;
        } else {
            arq_saida << "> Error, invalid control signals." << endl; 
        }
        arq_saida << "==============================================" << endl;

        //A = C;
        cont = pc + 1;

    }   

    arq_saida <<     "Cycle: " << cont + 1 << endl;
    arq_saida <<     "PC...: " << cont + 1 << endl;
    arq_saida <<     "> Line is empty, EOP." << endl;
    arq_saida.close();
}

int main() {
    uint32_t B = 0b10000000000000000000000000000000;
    uint32_t A = 0b00000000000000000000000000000001;

    ULA(A, B);

    return 0;
}