import java.util.concurrent.Semaphore;

public class Babuinos {

    private static final int INTERVALO_TEMPO = 2; // 1ms em milissegundos

    private static Semaphore mutexCorda = new Semaphore(1);
    private static int QUANTIDADE_BABUINOS = 0;
    private static int babuinosLesteAtuais = 0;
    private static int babuinosOesteAtuais = 0;

    static class GeradorBabuinosLeste extends Thread {
        Babuino babuinoLeste[];

        @Override
        public void run() {
            babuinoLeste = new Babuino[QUANTIDADE_BABUINOS];
            for (int i = 0; i < QUANTIDADE_BABUINOS; i++) {
                babuinoLeste[i] = new Babuino(i * 2, 'L');
                babuinoLeste[i].start();
                try {
                    Thread.sleep(INTERVALO_TEMPO);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            try {
                for (int i = 0; i < QUANTIDADE_BABUINOS; i++) {
                    try {
                        babuinoLeste[i].join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    static class GeradorBabuinosOeste extends Thread {
        Babuino babuinoOeste[];

        @Override
        public void run() {
            babuinoOeste = new Babuino[QUANTIDADE_BABUINOS];
            for (int i = 0; i < QUANTIDADE_BABUINOS; i++) {
                babuinoOeste[i] = new Babuino(i * 2 + 1, 'O');
                babuinoOeste[i].start();
                try {
                    Thread.sleep(INTERVALO_TEMPO);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            try {
                for (int i = 0; i < QUANTIDADE_BABUINOS; i++) {
                    try {
                        babuinoOeste[i].join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

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
                babuinoQuerAtravessar();
                mutexCorda.acquire();
                if (direcao == 'L') {
                    if (babuinosOesteAtuais == 0) {
                        pegarCorda();
                        babuinosLesteAtuais++;
                        soltarCorda();
                    }
                } else { // direcao == 'O'
                    if (babuinosLesteAtuais == 0) {
                        pegarCorda();
                        babuinosOesteAtuais++;
                        soltarCorda();
                    }
                }
                mutexCorda.release();
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
            if (direcao == 'L') {
                babuinosLesteAtuais--;
            } else {
                babuinosOesteAtuais--;
            }
        }
    }

    public static void main(String[] args) {
        QUANTIDADE_BABUINOS = Integer.parseInt(args[0]);
        if (QUANTIDADE_BABUINOS <= 0 || args.length != 1) {
            System.out.println("Quantidade invalida de babuinos. Por favor, insira um número de babuinos valido!");
            System.exit(1);
        }

        System.out.println("Inicio da simulação: " + QUANTIDADE_BABUINOS + " babuinos para cada lado");
        GeradorBabuinosLeste geradorLeste = new GeradorBabuinosLeste();
        GeradorBabuinosOeste geradorOeste = new GeradorBabuinosOeste();
        geradorLeste.start();
        geradorOeste.start();

        try {
            geradorLeste.join();
            geradorOeste.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Fim da simulação3");
    }
}
