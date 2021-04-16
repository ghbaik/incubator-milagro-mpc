/*
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
*/

/* GG20 Phase 3 ZKP challenge unit test */

#include <string.h>
#include "test.h"
#include "amcl/gg20_zkp.h"

#define LINE_LEN 1024
#define IDLEN 16
#define ADLEN 16

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: ./test_gg20_zkp_phase3_challenge [path to test vector file]\n");
        exit(EXIT_FAILURE);
    }

    int test_run = 0;

    FILE *fp;
    char line[LINE_LEN] = {0};

    const char *TESTline = "TEST = ";
    int testNo = 0;

    char v[GFS_SECP256K1 + 1];
    octet V = {0, sizeof(v), v};
    const char *Vline = "V = ";

    char c[GFS_SECP256K1 + 1];
    octet C = {0, sizeof(c), c};
    const char *Cline = "C = ";

    char e[GGS_SECP256K1];
    char e_golden[GGS_SECP256K1];
    octet E = {0, sizeof(e), e};
    octet E_GOLDEN = {0, sizeof(e_golden), e_golden};
    const char *Eline = "E = ";

    char id[IDLEN];
    octet ID = {0, sizeof(id), id};
    const char *IDline = "ID = ";

    char ad[IDLEN];
    octet AD = {0, sizeof(ad), ad};
    octet *AD_ptr = NULL;
    const char *ADline = "AD = ";

    // Line terminating a test vector
    const char *last_line = Eline;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("ERROR opening test vector file\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, LINE_LEN, fp) != NULL)
    {
        scan_int(&testNo, line, TESTline);

        scan_OCTET(fp, &V, line, Vline);
        scan_OCTET(fp, &C, line, Cline);

        scan_OCTET(fp, &ID, line, IDline);
        scan_OCTET(fp, &AD, line, ADline);

        scan_OCTET(fp, &E_GOLDEN, line, Eline);

        if (!strncmp(line, last_line, strlen(last_line)))
        {
            // Also input AD if it is not empty
            AD_ptr = NULL;
            if (AD.len > 0)
            {
                AD_ptr = &AD;
            }

            GG20_ZKP_phase3_challenge(&V, &C, &ID, AD_ptr, &E);

            compare_OCT(fp, testNo, "GG20_ZKP_phase3_challenge", &E, &E_GOLDEN);

            // Mark that at least one test vector was executed
            test_run = 1;
        }
    }

    fclose(fp);

    if (test_run == 0)
    {
        printf("ERROR no test vector was executed\n");
        exit(EXIT_FAILURE);
    }

    printf("SUCCESS\n");
    exit(EXIT_SUCCESS);
}
