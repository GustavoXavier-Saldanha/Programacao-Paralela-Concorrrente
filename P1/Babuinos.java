import java.util.concurrent.Semaphore;

public class Babuinos {

    private static final int INTERVALO_TEMPO = 1000; // 1ms em milissegundos

    private static Semaphore mutexCorda = new Semaphore(1);
    private static int babuinosLesteAtuais = 0;
    private static int babuinosOesteAtuais = 0;
    private static int QUANTIDADE_BABUINOS = 0;
    private static int babuinosAtravessados = 0;

    static class Babuino extends Thread {
        private int numero;
        private char direcao;

        public Babuino(int numero, char direcao) {
            this.numero = numero;
            this.direcao = direcao;
        }

        @Override
        public void run() {
            try {
                while (true) {
                    babuinoQuerAtravessar();
                    mutexCorda.acquire();
                    if ((direcao == 'L' && babuinosOesteAtuais == 0) || (direcao == 'O' && babuinosLesteAtuais == 0)) {
                        pegarCorda();
                        if (direcao == 'L') {
                            babuinosLesteAtuais++;
                        } else {
                            babuinosOesteAtuais++;
                        }
                        soltarCorda();

                        if (++babuinosAtravessados > QUANTIDADE_BABUINOS) {
                            mutexCorda.release();
                            break;
                        }
                    }
                    mutexCorda.release();
                    Thread.sleep(INTERVALO_TEMPO);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        private void babuinoQuerAtravessar() {
            System.out.println("Babuino " + numero + " quer atravessar para " + (direcao == 'L' ? "LESTE" : "OESTE"));
        }

        private void pegarCorda() {
            System.out.println(
                    ">>> BABUINO " + numero + " esta atravessando para " + (direcao == 'L' ? "LESTE" : "OESTE"));
            try {
                Thread.sleep(INTERVALO_TEMPO);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        private void soltarCorda() {
            System.out.println(" BABUINO " + numero + " chegou!");
        }
    }

    public static void main(String[] args) {
        QUANTIDADE_BABUINOS = Integer.parseInt(args[0]);
        if (args.length != 1 || QUANTIDADE_BABUINOS <= 0) {
            System.out.println(
                    "Quantidade invalida de babuinos. Por favor, insira um número de babuinos válida!");
            System.exit(1);
        }

        Babuino[] babuinos = new Babuino[QUANTIDADE_BABUINOS * 2];
        for (int i = 0; i < QUANTIDADE_BABUINOS; i++) {
            babuinos[i * 2] = new Babuino(i * 2, 'L'); //garantindo que os números dos babuínos que atravessam para o Leste sejam pares
            babuinos[i * 2 + 1] = new Babuino(i * 2 + 1, 'O'); //garantindo que os números dos babuínos que atravessam para o Oeste sejam ímpares
            babuinos[i * 2].start(); //Iniciamos a thread do babuíno que atravessa para o Leste
            babuinos[i * 2 + 1].start(); //Iniciamos a thread do babuíno que atravessa para o Oeste
            try {
                Thread.sleep(INTERVALO_TEMPO);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (Babuino babuino : babuinos) {
            try {
                babuino.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("Todos os babuinos atravessaram a corda!");
    }
}
