/** @file
 * @brief Ex. 8.3 TP6 dév.
 * @author rbardini
 * @date 9/11/2023
 *
 * consigne
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/** @struct
 * @brief Nombre de quatre chiffres binaires (1/2 octet) (Petit boutisme)
 */
typedef struct {
    bool c_1;
    bool c_2;
    bool c_3;
    bool c_4;
} t_quad;

/** @fn
 * @brief Addition de deux chiffres binaires
 * @param A premier chiffre
 * @param B second chiffre
 * @param Cin retenue en entrée
 * @param Cout retenue en sortie
 * @return S, le résultat de l'addition
 */
bool add_b(bool A, bool B, bool Cin, bool *Cout);

/** @fn
 * @brief Addition de deux nombre de quatre chiffres binaires
 * @param A premier nombre
 * @param B second nombre
 * @param D registre de débordement
 * @return S, le résultat de l'addition
 */
t_quad quad_add(t_quad A, t_quad B, bool *D);

/** @fn
 * @brief Saisie utilisateur d'un quad.
 * @return le quad saisi
 */
t_quad quad_saisir(void);

/** @fn
 * @brief Affichage d'un quad à l'écran
 * @param q le quad à afficher
 */
void quad_afficher(t_quad q);

/** @fn
 * @brief Conversion d'un quad en entier
 * @param q le quad a convertir
 * @return l'entier résultat de la conversion
 */
int quad_versEntier(t_quad q);

/** @fn
 * @brief Test de la fonction quad_add()
 */
void test_add_quad(void);

bool cas_test_add_quad(size_t numero, t_quad a, t_quad b);

int main()
{
    /*t_quad q1, q2;
    bool D;

    printf("q1 = ");
    q1 = quad_saisir();
    printf("q2 = ");
    q2 = quad_saisir();

    printf("q1 = ");
    quad_afficher(q1);
    printf("@nq2 = ");
    quad_afficher(q2);

    printf("@n@nrésultat = ");
    quad_afficher(quad_add(q1, q2, &D));
    printf("@nDébordement ? %d@n", D);*/

    test_add_quad();/**/

    return EXIT_SUCCESS;
}

t_quad quad_add(t_quad A, t_quad B, bool *D)
{
    t_quad S;
    bool C;

    S.c_1 = add_b(A.c_1, B.c_1, 0, &C);
    S.c_2 = add_b(A.c_2, B.c_2, C /*C1*/, &C);
    S.c_3 = add_b(A.c_3, B.c_3, C /*C2*/, &C);
    S.c_4 = add_b(A.c_4, B.c_4, C /*C3*/, D);

    return S;
}

int quad_versEntier(t_quad q)
{
    // clang-format off
    return q.c_1
        + 2 * q.c_2
        + 4 * q.c_3
        + 8 * q.c_4;
    // clang-format on
}

bool add_b(bool A, bool B, bool Cin, bool *Cout)
{
    bool A_OUex_B = A ^ B;

    *Cout = (A_OUex_B && Cin) || (B && A);

    return A_OUex_B ^ Cin;
}

t_quad quad_saisir(void)
{
    char c1, c2, c3, c4;
    bool cestBon = false;
    do {
        // clang-format off
        cestBon = scanf(" %c%c%c%c", &c4, &c3, &c2, &c1) == 4
                && (c1 == '0' || c1 == '1')
                && (c2 == '0' || c2 == '1')
                && (c3 == '0' || c3 == '1')
                && (c4 == '0' || c4 == '1');
        // clang-format on
        if (!cestBon) {
            printf("Erreur de saisie\n");
        }
    } while (!cestBon);

    return (t_quad){
        .c_1 = c1 == '1',
        .c_2 = c2 == '1',
        .c_3 = c3 == '1',
        .c_4 = c4 == '1',
    };
}

void quad_afficher(t_quad q)
{
    printf("%d%d%d%d", q.c_4, q.c_3, q.c_2, q.c_1);
}

/* PROCÉDURES DE TEST */

void test_add_quad(void)
{
#define SZ 16
    t_quad nombres[SZ] = {{0, 0, 0, 0},
                          {1, 0, 0, 0},
                          {0, 1, 0, 0},
                          {1, 1, 0, 0},
                          {0, 0, 1, 0},
                          {1, 0, 1, 0},
                          {0, 1, 1, 0},
                          {1, 1, 1, 0},
                          {0, 0, 0, 1},
                          {1, 0, 0, 1},
                          {0, 1, 0, 1},
                          {1, 1, 0, 1},
                          {0, 0, 1, 1},
                          {1, 0, 1, 1},
                          {0, 1, 1, 1},
                          {1, 1, 1, 1}};

    printf(" #  | %-7s | A    | B    | D | S\n", "Réussi?");

    int nbEchecs = 0;

    // Nombre de cas : 16 * 16 = 256
    for (size_t i = 0; i < SZ; ++i) {
        for (size_t j = 0; j < SZ; ++j) {
            if (!cas_test_add_quad(i * SZ + j, nombres[i], nombres[j])) {
                ++nbEchecs;
            }
        }
    }

    printf("\nTest de add_quad : %d échoués, %d réussis (%d total)\n", nbEchecs, (SZ * SZ) - nbEchecs, SZ * SZ);
#undef SZ
}

bool cas_test_add_quad(size_t numero, t_quad a, t_quad b)
{
        int entierAttendu = quad_versEntier(a) + quad_versEntier(b);
        bool deborde;
        t_quad recu = quad_add(a, b, &deborde);
        int entierRecu = quad_versEntier(recu);

        if (deborde) {
            entierAttendu -= 16;
        }

        bool reussi = entierAttendu == entierRecu;

        printf("%3zu | %-7s | ", numero, reussi ? "REUSSI" : "ÉCHEC");
        quad_afficher(a);
        printf(" | ");
        quad_afficher(b);
        printf(" | %d | ", deborde);
        quad_afficher(recu);

        if (!reussi) {
            printf(" (attendu %d)", entierAttendu);
        }

        printf("\n");

        return reussi;
}