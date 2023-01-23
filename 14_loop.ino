  void loop(){
  komunikacja_out();
  komunikacja_in();
  Stacja0.automat_stanu();
  Stacja1.automat_stanu();
  Stacja2.automat_stanu();
  Sortownia2.automat_stanu();
  Sortownia1.automat_stanu();
  Robot.rutyna();
  Linia_produkcyjna.automat_stanu();

  if(Stacja1.status_stacji==2)status_maszyny=2;

  if(Stacja2.status_stacji==2)status_maszyny=2;  

  if(Sortownia1.status_stacji==2)status_maszyny=2;

  if(Sortownia2.status_stacji==2)status_maszyny=2;
  
  if(Robot.status_stacji==2)status_maszyny=2;
  PC.print("master:");
  PC.print(Linia_produkcyjna.stan);
  PC.print(" Stacja1:");
  PC.print(Stacja1.stan);
  PC.print(" Stacja2:");
  PC.print(Stacja2.stan);
  PC.println();
  }
