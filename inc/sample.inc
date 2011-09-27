# Fixed example to print a random A, B, C

writeChars: block (out:char<-, c:char, count:int, done:bool<-)
   block (i:int)
      i = 0;
      while (i < count)
         out <- c;
         i = i + 1;
      endwhile
   endblock
   done <- true;
endblock

sinkBool: block (in:bool->)
   block (temp:bool)
      in -> temp;
   endblock
endblock

syncBool2: block (in1:bool->, in2:bool->, out:bool<-)
   par ()
      sinkBool (in:in1);
      sinkBool (in:in2);
   endpar
   out <- true;
endblock

syncBool3: block (in1:bool->, in2:bool->, in3:bool->, out:bool<-)
   par (temp:bool<>)
      syncBool2 (in1:in1, in2:in2, out:temp);
      syncBool2 (in1:temp, in2:in3, out:out);
   endpar
endblock

combineChars2: block (in1:char->, in2:char->, out:char<-, stop:bool->,
                      done:bool<-)
   block (temp:char, stopped:bool)
      stopped = false;
      while (not stopped)
         alt (in1 -> temp)
            out <- temp;
         elsealt (in2 -> temp)
            out <- temp;
         elsealt (stop -> stopped)
         endalt
      endwhile
   endblock
   done <- true;
endblock

sourceBool: block (out:bool<-, b:bool)
   out <- b;
endblock

splitBool2: block (in:bool->, out1:bool<-, out2:bool<-)
   block (temp:bool)
      in -> temp;
      par ()
         sourceBool (out:out1, b:temp);
         sourceBool (out:out2, b:temp);
      endpar
   endblock
endblock

combineChars3: block (in1:char->, in2:char->, in3:char->, out:char<-,
                      stop:bool->, done:bool<-)
   par (done1:bool<>, done2:bool<>, stop1:bool<>, stop2:bool<>, temp:char<>)
      combineChars2 (in1:in1, in2:in2, out:temp, stop:stop1, done:done1);
      combineChars2 (in1:temp, in2:in3, out:out, stop:stop2, done:done2);
      splitBool2 (in:stop, out1:stop1, out2:stop2);
      syncBool2 (in1:done1, in2:done2, out:done);
   endpar
endblock

screen: block (in:char->, stop:bool->)
   block (stopped:bool, temp:char)
      stopped = false;
      while (not stopped)
         alt (in -> temp)
            $ putchar(temp_);
            $ fflush(stdout);
         elsealt (stop -> stopped)
         endalt
      endwhile
   endblock
endblock

main: block ()
   block (A:char, B:char, C:char, count:int)
      A = 'A';
      B = 'B';
      C = 'C';
      count = 10;
      par (c1:char<>, c2:char<>, c3:char<>, c4:char<>,
           d1:bool<>, d2:bool<>, d3:bool<>, d4:bool<>, d5:bool<>)
         writeChars (out:c1, c:A, count:count, done:d1);
         writeChars (out:c2, c:B, count:count, done:d2);
         writeChars (out:c3, c:C, count:count, done:d3);
         syncBool3 (in1:d1, in2:d2, in3:d3, out:d4);
         combineChars3 (in1:c1, in2:c2, in3:c3, out:c4, stop:d4, done:d5);
         screen(in:c4, stop:d5);
      endpar
   endblock
endblock
