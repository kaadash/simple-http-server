char* getFilePath(char *path, int offset) {
  int i = offset;
  char *output = malloc (sizeof (char) * 100);
  while (path[i] != ' ') {
    output[i - offset] = path[i];
    i++;
  }
  return output;
}

char* concat(char* str1, char* str2) {
  char * output = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
  strcpy(output, str1);
  strcat(output, str2);
  return output;
}

int isBlank(char *line) {
  char* ch;
  int isBlankLine = 1;
  printf("%s\n", line);
  for (ch = line; *ch != '\0'; ++ch)
  {
    if (!isspace(*ch))
    {
      isBlankLine = 0;
      break;
    }
  }

  return isBlankLine;
}

int checkIfFileExist(char* fileName) {
  if( access( fileName, F_OK ) != -1 ) {
    return 1;
  } else {
    return 0;
  }
}

int getFileSize(FILE* fp) {
  fseek(fp, 0, SEEK_END);
  int fileSize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return fileSize;
}

int isEndOfHeader(char* header) {
  int size = strlen(header);
  if (size < 4) {
    return 0;
  } else {
    if (header[size - 4] == '\r' && header[size - 3] == '\n' &&
      header[size - 2] == '\r' && header[size - 1] == '\n') {
      return 1;      
    } else {
      return 0;      
    }
  }
}

int getContentLength(char* buf) {
  char *s;
  s = strstr(buf, "Content-Length: ");
  char result[100];
  int iter = 0;
  if (s != NULL)
  {
     printf("Found string at index = %ld\n", s - buf);
     int index = s - buf;
     index += 16;
     while (buf[index] != '\n') {
      result[iter] = buf[index];
      printf("%c\n", buf[index]);
      index++;
      iter++;
     }
  }
  else
  {
     printf("String not found\n");
 }
 return atoi(result);
}
