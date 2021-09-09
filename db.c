#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

typedef struct {
  StatementType type;
} Statement;

typedef struct {
  char* buffer;
  size_t bufferLength;
  ssize_t inputLength;
} InputBuffer;

InputBuffer* newInputBuffer() {
  InputBuffer* inputBuffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  inputBuffer->buffer = NULL;
  inputBuffer->bufferLength = 0;
  inputBuffer->inputLength = 0;

  return inputBuffer;
}

void readInput(InputBuffer* inputBuffer) {
  ssize_t bytesRead = getline(&(inputBuffer->buffer), &(inputBuffer->bufferLength), stdin);
  
  if (bytesRead <= 0) {
    printf("Error reading input.\n");
    exit(EXIT_FAILURE);
  }

  inputBuffer->buffer[bytesRead - 1] = 0;
  inputBuffer->inputLength = bytesRead - 1;
}

void closeInputBuffer(InputBuffer* inputBuffer) {
  free(inputBuffer->buffer);
  free(inputBuffer);
}

void printPrompt() {
  printf("db > ");
}

MetaCommandResult doMetaCommand(InputBuffer* inputBuffer) {
  if (strcmp(inputBuffer->buffer, ".exit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
  if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(inputBuffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void executeStatement(Statement* statement) {
  switch(statement->type) {
    case (STATEMENT_INSERT):
      printf("This is where we would do an insert.\n");
      break;
    case (STATEMENT_SELECT):
      printf("This is where we would do a select.\n");
      break;
  }
}

int main(int argc, char* argv[]) {
  InputBuffer* inputBuffer = newInputBuffer();
  while (true) {
    printPrompt();
    readInput(inputBuffer);

    if (inputBuffer->buffer[0] == '.') {
      switch (doMetaCommand(inputBuffer)) {
	case (META_COMMAND_SUCCESS):
	  continue;
	case (META_COMMAND_UNRECOGNIZED_COMMAND):
	  printf("Unrecognized command '%s'\n", inputBuffer->buffer);
	  continue;
      }
    }

    Statement statement;
    switch(prepareStatement(inputBuffer, &statement)) {
      case (PREPARE_SUCCESS):
	break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
	printf("Unrecognized keyword at start of '%s'.\n", inputBuffer->buffer);
	continue;
    }

    executeStatement(&statement);
    printf("Executed.\n");
  }
}
