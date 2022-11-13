/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *node = tree;

  // Iterate the tree
  while(node) {
    if (key == node->key) {
      *value = node->value;
      return true;
    };

    if (key < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  // Initialise new node
  bst_node_t *newNode = malloc(sizeof(bst_node_t));
  newNode->key = key;
  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;

  // If tree is empty, insert first node
  if (*tree == NULL) {
    *tree = newNode;
    return;
  }

  // Find parent node
  bst_node_t *node = *tree;
  while (node != NULL) {
    // If node key matches, replace the value
    if (key == node->key) {
      node->value = value;
      free(newNode);
      break;
    }

    // If key is smaller, set the node to left child
    if (key < node->key) {
      // If there is no further left node, insert
      if (!node->left) {
        node->left = newNode;
        return;
      } 

      node = node->left;
    }

    // Otherwise, set the node to right child and continue loop
    if (key > node->key) {
      if (!node->right) {
        node->right = newNode;
        return;
      }

      node = node->right;
    }
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *parent = *tree;
  bst_node_t *rightestNode = *tree;

  // Find the rightest node
  while (rightestNode->right) {
    parent = rightestNode;
    rightestNode = rightestNode->right;
  }

  // Set the target node values to rightest node values
  target->value = rightestNode->value;
  target->key = rightestNode->key;

  // Correctly set the parent node children
  if (parent != rightestNode) parent->right = rightestNode->left;
  else target->left = rightestNode->left;

  free(rightestNode);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (!*tree) return;

  bst_node_t *parent = *tree;
  bst_node_t *node = *tree;

  // Iterate the tree - find node with key
  while(node) {
    if (key == node->key) {
      break;
    };

    parent = node;

    if (key < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }

  // If we found the node
  if (node) {
    // If both subtrees exist
    if (node->left && node->right) {
      bst_replace_by_rightmost(node, &(node->left));
      return;
    }
    // If both subtrees don't exist
    if (!node->left && !node->right) {
      // Set the parent pointer to node to NULL
      if (parent->left == node) {
        parent->left = NULL;
      }
      if (parent->right == node) {
        parent->right = NULL;
      }
      free(node);
    }
    // If only one subtree exists
    if (parent->left == node) {
      if (node->left) parent->left = node->left;
      if (node->right) parent->left = node->right;
      free(node);
    }
    if (parent->right == node) {
      if (node->left) parent->right = node->left;
      if (node->right) parent->right = node->right;
      free(node);
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_node_t *node = *tree;

  // Iterate the tree to left and push all nodes to stack
  while (node) {
    stack_bst_push(&stack, node);
    node = node->left;
  }

  // While stack isn't empty
  while (!stack_bst_empty(&stack)) {
    node = stack_bst_pop(&stack);

    // If there is a right node, iterate to left and push all nodes to stack
    if (node->right) {
      bst_node_t *tmp = node->right;

      while(tmp != NULL) {
        stack_bst_push(&stack, tmp);
        tmp = tmp->left;
      }
    }
    
    // Free each node
    free(node);
  }
  *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *node = tree;

  while (node) {
    bst_print_node(node);
    stack_bst_push(to_visit, node);
    node = node->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_preorder(tree, &stack);
  bst_node_t *node;

  while (!stack_bst_empty(&stack)) {
    node = stack_bst_pop(&stack);
    bst_leftmost_preorder(node->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *node = tree;

  while (node) {
    stack_bst_push(to_visit, node);
    node = node->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_inorder(tree, &stack);
  bst_node_t *node;

  while (!stack_bst_empty(&stack)) {
    node = stack_bst_pop(&stack);
    bst_print_node(node);
    bst_leftmost_inorder(node->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  bst_node_t *node = tree;

  while (node) {
    stack_bst_push(to_visit, node);
    stack_bool_push(first_visit, true);
    node = node->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  stack_bst_t stack_bst;
  stack_bst_init(&stack_bst);

  stack_bool_t stack_bool;
  stack_bool_init(&stack_bool);

  bst_node_t *node;
  bst_leftmost_postorder(tree, &stack_bst, &stack_bool);

  while (!stack_bst_empty(&stack_bst)) {
    node = stack_bst_pop(&stack_bst);
    bool first_visit = stack_bool_pop(&stack_bool);

    if (first_visit) {
      stack_bst_push(&stack_bst, node);
      stack_bool_push(&stack_bool, false);
      bst_leftmost_postorder(node->right, &stack_bst, &stack_bool);
    } else {
      bst_print_node(node);
    }
  }
}
