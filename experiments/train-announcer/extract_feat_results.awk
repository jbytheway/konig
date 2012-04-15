{
  printf("%s ", $1);
  done = 0;
  for (i=4; i<=NF; i+=4) {
    this_feat=$(i+3);
    if (this_feat != feat) continue;
    unannounced = $(i+1)+0;
    announced = $(i+2)+0;
    if (announced > unannounced) {
      printf("TRUE\n");
      done = 1;
      break;
    }
  }
  if (!done) {
    printf("FALSE\n");
  }
}

