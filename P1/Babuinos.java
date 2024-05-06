public class Babuinos {
    private static int QUANTIDADE_BABUINOS = 0;

    static class Travessia {
        // L --> indo leste
        // O --> indo oeste
        // V --> vazio
        private int quantidadeAtravessando = 0;
        private char ladoAtual = 'V';

        // leste quer ir pra Oeste
        public synchronized boolean lesteQuerAtravessar() {
            if (this.ladoAtual == 'L') {
                return false;
            }
            this.quantidadeAtravessando++;
            this.ladoAtual = 'O';
            return true;
        }

        // oeste quer ir pra Leste
        public synchronized boolean oesteQuerAtravessar() {
            if (this.ladoAtual == 'O') {
                return false;
            }
            this.quantidadeAtravessando++;
            this.ladoAtual = 'L';
            return true;
        }

        public synchronized void notificar() {
            if (this.quantidadeAtravessando == 1) {
                this.ladoAtual = 'V';
            }
            this.quantidadeAtravessando--;
            notifyAll();
        }

        public synchronized void dormir() {
            try {
                wait();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

    }

    static class GeradorBabuinosLeste extends Thread {
        Babuino babuinoLeste[];
        Travessia corda;

        public GeradorBabuinosLeste(Travessia corda) {
            this.corda = corda;
        }

        @Override
        public void run() {
            babuinoLeste = new Babuino[QUANTIDADE_BABUINOS];
            for (int i = 0; i < QUANTIDADE_BABUINOS; i++) {
                babuinoLeste[i] = new Babuino(i * 2, 'L', this.corda);
                babuinoLeste[i].start();
                try {
                    long intervaloTempo = (long) (Math.random() * 4) + 1;
                    sleep(intervaloTempo);
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
        Travessia corda;

        public GeradorBabuinosOeste(Travessia corda) {
            this.corda = corda;
        }

        @Override
        public void run() {
            babuinoOeste = new Babuino[QUANTIDADE_BABUINOS];
            for (int i = 0; i < QUANTIDADE_BABUINOS; i++) {
                babuinoOeste[i] = new Babuino(i * 2 + 1, 'O', this.corda);
                babuinoOeste[i].start();
                try {
                    long intervaloTempo = (long) (Math.random() * 4) + 1;
                    sleep(intervaloTempo);
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
        private Travessia corda;

        public Babuino(int numero, char direcao, Travessia corda) {
            this.numero = numero;
            this.direcao = direcao;
            this.corda = corda;
        }

        @Override
        public void run() {
            try {
                System.out
                        .println("Babuino " + numero + " quer atravessar para " + (direcao == 'L' ? "LESTE" : "OESTE"));

                babuinoQuerAtravessar();

                pegarCorda();

                // simular tempo para atravessar
                sleep(1);

                soltarCorda();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        private void babuinoQuerAtravessar() {
            if (this.direcao == 'L') {
                while (!this.corda.oesteQuerAtravessar()) {
                    this.corda.dormir();
                }
            } else {
                while (!this.corda.lesteQuerAtravessar()) {
                    this.corda.dormir();
                }
            }
        }

        private void pegarCorda() {
            System.out.println(
                    ">>> BABUINO " + numero + " esta atravessando para " + (direcao == 'L' ? "LESTE" : "OESTE"));

        }

        private void soltarCorda() {
            System.out.println(" BABUINO " + numero + " chegou!");

            this.corda.notificar();
        }
    }

    public static void main(String[] args) {

        QUANTIDADE_BABUINOS = Integer.parseInt(args[0]);
        if (QUANTIDADE_BABUINOS <= 0 || args.length != 1) {
            System.out.println("Quantidade invalida de babuinos. Por favor, insira um número de babuinos valido!");
            System.exit(1);
        }

        Travessia corda = new Travessia();

        System.out.println("Inicio da simulacao: " + QUANTIDADE_BABUINOS + " babuinos para cada lado");
        GeradorBabuinosLeste geradorLeste = new GeradorBabuinosLeste(corda);
        GeradorBabuinosOeste geradorOeste = new GeradorBabuinosOeste(corda);
        geradorLeste.start();
        geradorOeste.start();

        try {
            geradorLeste.join();
            geradorOeste.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Fim da simulacao!");
    }
}
