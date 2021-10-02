/* Lucas Roberto Raineri Oliveira - 38346 */
/* Codigo formatado utilizando o clang-format no formato LLVM*/

#include <stdio.h>

#include "gfx/gfx.h"

/* Constantes */
enum {
  INSERT,
  REMOVE,
  SEARCH,
  DUMP_TREE,
  READ_TREE,
  EXIT,
  EMPTY_TREE,
  FOUND,
  LEFT_NOT_FOUND,
  RIGHT_NOT_FOUND
};

#define FONT_SIZE 30

/* Pra conformar com os algoritmos... */
#define TRUE 1
#define FALSE 0

/* O tamanho da janela*/
#define tamX 1280
#define tamY 720

/* Clear pros SOs suportados */
#ifndef _WIN32
#define clear() system("clear");
#else
#define clear() system("cls");
#endif

typedef struct nodeProto {
  int32_t key : 28;
  int32_t val : 2;
  int32_t reserved : 2;
  struct nodeProto *left, *right;
} node;

typedef struct avlTreeProto {
  node *root;
} avlTree;

typedef struct fileNodeProto {
  int32_t key : 28;
  int32_t val : 2;
  uint32_t left : 1, right : 1;
} fileNode;

/* NOTE Funcoes de insercao da AVL */
/* Insere WHO na arvore */
void insertNode(node **root, int who, unsigned int *h);
/* Equivalente a inicio-no */
void initNode(node **ptr, int x);
/* Casos 1 e 2 definidos no algoritmo */
void case1(node **ptr, unsigned int *h);
void case2(node **ptr, unsigned int *h);

/* NOTE Funcoes de Remocao */
/* Deleta um no da arvore que coincide com o numero de who, nao faz nada se nao
 * achar */
void deleteNode(avlTree *tree, node *who);
/* Libera todos os nos da arvore recursivamente */
void freeTree(node *root);

/* NOTE Funcoes auxiliares */
/* substitui a subarvore com raiz em u pela com raiz em v */
void transplant(avlTree *tree, node *u, node *v);
/* Procura um no com o valor de who, retorna o ponteiro e um codigo de retorno,
 * definido no enum */
void search(node **root, int who, int *returnCode);
/* Retorna o no com menor valor (mais a esquerda) a partir de uma subarvore root
 */
node *tree_min(node *root);
/* Retorna o no com maior valor (mais a direita) a partir de uma subarvore root
 */
node *tree_max(node *root);
node *getParent(node *child, node *root);

/* NOTE Funcoes de desenho */
void drawTree(avlTree *tree);
void drawNode(node *which, int x, int y);
void recursiveDrawFromRoot(node *root, int *depthX, int *depthY,
                           node *treeRoot);

void readTreeFromFile(FILE *stream, node **rootPtr);
void saveTreeToFile(node *root, FILE *treeBin);

int main(int argc, char *argv[]) {
  argc = argc;
  argv[0] = "Arvore AVL";
  avlTree tree;
  node *ptr;
  int retCode, opc, aux;
  unsigned int h;
  FILE *binTree;

  h = FALSE;
  tree.root = NULL;
  gfx_init(tamX, tamY, "Arvore de busca");

  for (;;) {
    ptr = tree.root;
    do {
      printf("Informe o que quer fazer na arvore\n\t0 - Insercao\n\t1 - "
             "Remocao\n\t2 - Procura\n\t3 - Salvar arvore atual para "
             "\"tree.bin\"\n\t4 - Ler arvore de \"tree.bin\"\n\t5 - Sair\n");
      scanf("%d", &opc);
      clear();
    } while (opc < INSERT || opc > EXIT);
    if (opc == EXIT)
      break;
    switch (opc) {
    case INSERT:
      printf("Informe o termo inteiro a ser inserido.\n");
      scanf("%d", &aux);
      search(&ptr, aux, &retCode);
      if (retCode != FOUND) {
        insertNode(&tree.root, aux, &h);
        printf("O termo %d foi inserido com sucesso!\n", aux);
      } else {
        printf("O termo ja existe na arvore.\n");
      }
      drawTree(&tree);
      break;
    case REMOVE:
      printf("Informe o termo a ser removido.\n");
      scanf("%d", &aux);
      search(&ptr, aux, &retCode);
      /* Se nao achou um termo com esse numero, ignorar */
      if (retCode != FOUND) {
        printf("Termo nao encontrado!\n");
      } else {
        /* Se achou um termo com esse numero deleta o no da arvore e desaloca
         * seu bloco de memoria */
        deleteNode(&tree, ptr);
        ptr->left = NULL;
        ptr->right = NULL;
        ptr->val = 0;
        free(ptr);
        printf("O termo %d foi removido com sucesso!\n", aux);
      }
      drawTree(&tree);
      break;
    case SEARCH:
      printf("Informe o termo a ser procurado.\n");
      scanf("%d", &aux);
      search(&ptr, aux, &retCode);
      if (retCode != FOUND) {
        printf("Termo nao encontrado!\n");
      } else {
        printf("No encontrado! Tem como valor %d.\n", ptr->key);
      }
      break;
    case DUMP_TREE:
      binTree = fopen("tree.bin", "wb+");
      saveTreeToFile(tree.root, binTree);
      fclose(binTree);
      break;
    case READ_TREE:
      binTree = fopen("tree.bin", "rb");
      if (binTree != NULL) {
        readTreeFromFile(binTree, &tree.root);
        fclose(binTree);
        drawTree(&tree);
      }
      printf("\"tree.bin\" nao existe!\n");
      break;
    default:
      break;
    }
  }

  /* Finalizacao do programa */
  freeTree(tree.root);
  tree.root = NULL;
  gfx_quit();

  return 0;
}

/* NOTE: usando os algoritmos passados em aula, conforme instruido */
void insertNode(node **root, int who, unsigned int *h) {
  if ((*root) == NULL) {
    initNode(root, who);
    *h = TRUE;
  } else {
    if (who == (*root)->key) {
      return;
    }
    if (who < (*root)->key) {
      insertNode(&(*root)->left, who, h);
      if (*h) {
        switch ((*root)->val) {
        case 1:
          (*root)->val = 0;
          *h = FALSE;
          break;
        case 0:
          (*root)->val = -1;
          break;
        case -1:
          case1(&(*root), h);
          break;
        default:
          break;
        }
      }
    } else {
      insertNode(&(*root)->right, who, h);
      if (*h) {
        switch ((*root)->val) {
        case -1:
          (*root)->val = 0;
          *h = FALSE;
          break;
        case 0:
          (*root)->val = 1;
          break;
        case 1:
          case2(&(*root), h);
          break;
        default:
          break;
        }
      }
    }
  }
}

void freeTree(node *root) {
  if (root != NULL) {
    /* Chamar pra todos os nos da direita */
    if (root->right != NULL) {
      freeTree(root->right);
    }
    /* Chamar pra todos os nos da esquerda */
    if (root->left != NULL) {
      freeTree(root->left);
    }
    /* Se chegou no final da arvore, se liberar */
    free(root);
    /* Retornar se ambos os galhos do no forem nulos */
  }
}

/* NOTE: usando os algoritmos passados em aula, conforme instruido */
void search(node **root, int who, int *returnCode) {
  /* Stack goes BRRRRRR */
  node *localNode;
  localNode = *root;
  if (*root == NULL) {
    *returnCode = EMPTY_TREE;
  } else if (who == localNode->key) {
    *returnCode = FOUND;
  } else if (who < localNode->key) {
    if (localNode->left == NULL) {
      *returnCode = LEFT_NOT_FOUND;
    } else {
      *root = localNode->left;
      search(root, who, returnCode);
    }
  } else if (localNode->right == NULL) {
    *returnCode = RIGHT_NOT_FOUND;
  } else {
    *root = localNode->right;
    search(root, who, returnCode);
  }
}

void deleteNode(avlTree *tree, node *z) {
  /* Se o da direita for vazio... */
  if (z->left == NULL) {
    /* Trocar this por this->right */
    transplant(tree, z, z->right);
  } else if (z->right == NULL) {
    transplant(tree, z, z->left);
  } else {
    node *y, *auxParent;
    y = tree_min(z->right);
    auxParent = getParent(y, tree->root);
    if (auxParent != z) {
      transplant(tree, y, y->right);
      y->right = z->right;
      auxParent = getParent(y->right, tree->root);
      auxParent = y;
    }
    transplant(tree, z, y);
    y->left = z->left;
    auxParent = getParent(y->left, tree->root);
    auxParent = y;
  }
}

/* NOTE: usando os algoritmos passados em aula, conforme instruido */
void transplant(avlTree *tree, node *u, node *v) {
  if (getParent(u, tree->root) == NULL) {
    tree->root = v;
  } else if (u == getParent(u, tree->root)->left) {
    getParent(u, tree->root)->left = v;
  } else
    getParent(u, tree->root)->right = v;
}

/* NOTE: como nao tinha implementacao desses metodos nos slides, eu fiz o meu
 * proprio */
node *tree_min(node *root) {
  if (root->left == NULL)
    return root;
  return tree_min(root->left);
}

/* NOTE: como nao tinha implementacao desses metodos nos slides, eu fiz o meu
 * proprio */
node *tree_max(node *root) {
  if (root->right == NULL)
    return root;
  return tree_min(root->right);
}

void drawTree(avlTree *tree) {
  /* As minhas projecoes de calculo servem para uma profundidade maxima de 5 p/
   * uma resolucao de 1280x720, eu podia melhorar isso fazendo com que o usuario
   * consiguisse redimensionar o viewport direto da tela com o mouse wheel up ou
   * down, mas teria que implementar isso diretamente na integracao com a SDL, o
   * que provavelmente seria uma dor de cabeca enorme, julgo uma profundidade de
   * 5 suficiente para os testes a serem realizados */
  int localDepthX, localDepthY;

  localDepthX = 0;
  localDepthY = 0;
  gfx_clear();
  recursiveDrawFromRoot(tree->root, &localDepthX, &localDepthY, tree->root);
  gfx_paint();
}

void drawNode(node *which, int x, int y) {
  char buffer[32];
  /* Background */
  gfx_set_color(128, 128, 128);
  gfx_filled_ellipse(x, y, 56, 56);
  /* Contorno */
  gfx_set_color(255, 255, 255);
  gfx_ellipse(x, y, 56, 56);
  /* Texto */
  gfx_set_font_size(FONT_SIZE);
  gfx_set_color(255, 0, 0);
  sprintf(buffer, "%d|%d", which->key, which->val);
  gfx_text(x - 28, y - 20, buffer);
}

void recursiveDrawFromRoot(node *root, int *depthX, int *depthY,
                           node *treeRoot) {
  int sizeX, sizeY;
  sizeX = (tamX / 2) + ((*depthX * 112) + 10);
  sizeY = 56 + ((*depthY * 112) + 10);
  if (root != NULL) {
    if (root->right != NULL) {
      gfx_set_color(255, 255, 255);
      if (getParent(root, treeRoot) == NULL) {
        *depthX += 3;
        gfx_line(sizeX, sizeY, (sizeX + 75) + 224, sizeY + 75);
      } else {
        *depthX += 1;
        gfx_line(sizeX, sizeY, sizeX + 75, sizeY + 75);
      }
      *depthY += 1;
      recursiveDrawFromRoot(root->right, depthX, depthY, treeRoot);
    }
    if (root->left != NULL) {
      gfx_set_color(255, 255, 255);
      if (getParent(root, treeRoot) == NULL) {
        *depthX -= 3;
        gfx_line(sizeX, sizeY, (sizeX - 75) - 224, sizeY + 75);
      } else {
        *depthX -= 1;
        gfx_line(sizeX, sizeY, sizeX - 75, sizeY + 75);
      }
      *depthY += 1;
      recursiveDrawFromRoot(root->left, depthX, depthY, treeRoot);
    }
    /* Volta desenhando... */
    drawNode(root, sizeX, sizeY);
    *depthY -= 1;
    /* Se esse cara nao for a raiz da arvore geral... */
    if (getParent(root, treeRoot) != NULL) {
      /* Se eu sou filho da raiz... */
      if (getParent(getParent(root, treeRoot), treeRoot) == NULL) {
        if (getParent(root, treeRoot)->left == root)
          *depthX += 3;
        else
          *depthX -= 3;
      } else {
        if (getParent(root, treeRoot)->left == root)
          *depthX += 1;
        else
          *depthX -= 1;
      }
    }
  } else {
    gfx_clear();
  }
}

node *getParent(node *child, node *root) {
  if (child == root) {
    return NULL;
  }
  if (root->key < child->key) {
    if (root->right == child) {
      return root;
    } else {
      return getParent(child, root->right);
    }
  } else if (root->key > child->key) {
    if (root->left == child) {
      return root;
    } else {
      return getParent(child, root->left);
    }
  }
  return NULL;
}

void initNode(node **ptr, int x) {
  *ptr = malloc(sizeof(node));
  (*ptr)->left = NULL;
  (*ptr)->right = NULL;
  (*ptr)->key = x;
  (*ptr)->val = 0;
}

void case1(node **ptr, unsigned int *h) {
  node *ptu, *ptv;
  ptu = (*ptr)->left;
  
  if (ptu->val == -1) {
    (*ptr)->left = ptu->right;
    ptu->right = (*ptr);
    (*ptr)->val = 0;
    (*ptr) = ptu;
  } else {
    ptv = ptu->right;
    ptu->right = ptv->left;
    ptv->left = ptu;
    (*ptr)->left = ptv->right;
    ptv->right = (*ptr);
    if (ptv->val == -1)
      (*ptr)->val = 1;
    else
      (*ptr)->val = 0;
    if (ptv->val == 1)
      ptu->val = -1;
    else
      ptu->val = 0;
    (*ptr) = ptv;
  }
  (*ptr)->val = 0;
  *h = FALSE;
}

void case2(node **ptr, unsigned int *h) {
  node *ptu, *ptv;
  ptu = (*ptr)->right;

  if (ptu->val == 1) {
    (*ptr)->right = ptu->left;
    ptu->left = (*ptr);
    (*ptr)->val = 0;
    (*ptr) = ptu;
  } else {
    ptv = ptu->left;
    ptu->left = ptv->right;
    ptv->right = ptu;
    (*ptr)->right = ptv->left;
    ptv->left = (*ptr);
    if (ptv->val == 1)
      (*ptr)->val = -1;
    else
      (*ptr)->val = 0;
    if (ptv->val == -1)
      ptu->val = 1;
    else
      ptu->val = 0;
    (*ptr) = ptv;
  }
  (*ptr)->val = 0;
  *h = FALSE;
}

void readTreeFromFile(FILE *stream, node **rootPtr) {
  fileNode *temp;

  if ((temp = malloc(sizeof(fileNode))) == NULL) {
    /* Erro na alocacao */
    return;
  }

  if (fread(temp, sizeof(fileNode), 1, stream) == 0) {
    /* Final do arquivo */
    return;
  }

  if (((*rootPtr) = malloc(sizeof(node))) == NULL) {
    /* Erro na alocacao */
    return;
  }

  (*rootPtr)->val = temp->val;
  (*rootPtr)->key = temp->key;

  if (temp->right) {
    readTreeFromFile(stream, &(*rootPtr)->right);
  } else {
    (*rootPtr)->right = NULL;
  }

  if (temp->left) {
    readTreeFromFile(stream, &(*rootPtr)->left);
  } else {
    (*rootPtr)->left = NULL;
  }

  free(temp);
}

void saveTreeToFile(node *root, FILE *treeBin) {
  fileNode *temp;
  temp = NULL;
  if (root == NULL)
    return;
  if ((temp = malloc(sizeof(fileNode))) == NULL)
    return;
  temp->left = root->left != NULL ? TRUE : FALSE;
  temp->right = root->right != NULL ? TRUE : FALSE;
  temp->key = root->key;
  temp->val = root->val;
  fwrite(temp, sizeof(fileNode), 1, treeBin);
  free(temp);
  saveTreeToFile(root->right, treeBin);
  saveTreeToFile(root->left, treeBin);
}
