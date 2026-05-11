import React from 'react';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import styles from './index.module.css';

const features = [
  {
    icon: '⚡',
    title: 'Fast by Default',
    desc: 'Release builds use -O3 + LTO + -march=native. Software FPS cap with sub-millisecond precision keeps frame pacing tight.',
  },
  {
    icon: '🖥️',
    title: 'Smart Scaling',
    desc: 'Auto-fit or integer 1×/2×/3× scaling with aspect-correct letterboxing. Crisp pixels, no blur, no stretch.',
  },
  {
    icon: '🎮',
    title: 'Better Input',
    desc: '32-entry key queue (2× upstream), full mouse look support, runtime fullscreen toggle with F11.',
  },
  {
    icon: '⚙️',
    title: 'Zero Recompile Config',
    desc: 'All options via environment variables — scale, FPS cap, VSync, fullscreen, mouse. No rebuilds needed.',
  },
  {
    icon: '🔧',
    title: 'Easy to Port',
    desc: 'Implement 6 clean C functions to target any platform. The API is fully documented with typed headers.',
  },
  {
    icon: '🚀',
    title: 'GitHub CI/CD',
    desc: 'Automated Linux + macOS builds, CodeQL security analysis, and binary releases on every tagged push.',
  },
];

function HeroSection() {
  return (
    <div className={styles.hero}>
      <div className={styles.heroGlow} />
      <div className={styles.heroContent}>
        <div className={styles.badge}>v1.0 · Open Source · GPLv2</div>
        <h1 className={styles.heroTitle}>iodoom</h1>
        <p className={styles.heroSubtitle}>
          A fast, user-friendly Doom port for modern systems.
          <br />
          Built on doomgeneric — redesigned for real use.
        </p>
        <div className={styles.heroCta}>
          <Link className={styles.btnPrimary} to="/docs/intro">
            Get Started →
          </Link>
          <Link
            className={styles.btnSecondary}
            href="https://github.com/crishacks/iodoom"
          >
            View on GitHub
          </Link>
        </div>
        <div className={styles.heroCode}>
          <code>
            <span className={styles.codeComment}># Quick install</span>
            {'\n'}
            <span className={styles.codePunct}>$</span>{' '}
            <span className={styles.codeCmd}>./scripts/install.sh</span>{' '}
            <span className={styles.codeArg}>--wad doom.wad</span>
          </code>
        </div>
      </div>
    </div>
  );
}

function FeaturesSection() {
  return (
    <section className={styles.features}>
      <div className="container">
        <h2 className={styles.sectionTitle}>Why iodoom?</h2>
        <p className={styles.sectionSubtitle}>
          Everything doomgeneric should have been.
        </p>
        <div className={styles.featureGrid}>
          {features.map((f) => (
            <div key={f.title} className={styles.featureCard}>
              <span className={styles.featureIcon}>{f.icon}</span>
              <h3>{f.title}</h3>
              <p>{f.desc}</p>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}

function QuickStartSection() {
  return (
    <section className={styles.quickstart}>
      <div className="container">
        <div className={styles.quickstartInner}>
          <div className={styles.quickstartText}>
            <h2>Up and running in minutes</h2>
            <p>
              One script handles deps, build, and install.
              Or build manually in under a minute.
            </p>
            <Link className={styles.btnPrimary} to="/docs/building">
              Full build guide →
            </Link>
          </div>
          <div className={styles.quickstartCode}>
            <div className={styles.codeWindow}>
              <div className={styles.codeWindowHeader}>
                <span className={styles.dot} style={{background:'#ff5f57'}} />
                <span className={styles.dot} style={{background:'#ffbd2e'}} />
                <span className={styles.dot} style={{background:'#28ca41'}} />
                <span className={styles.codeWindowTitle}>terminal</span>
              </div>
              <pre className={styles.codeWindowBody}>{`# Linux
sudo apt install libsdl2-dev
cd src && make sdl RELEASE=1 -j$(nproc)

# macOS
brew install sdl2
cd src && make sdl RELEASE=1

# Play
./iodoom -iwad /path/to/doom.wad

# Options (no recompile)
IODOOM_SCALE=2 IODOOM_MOUSE=1 \\
  ./iodoom -iwad doom.wad`}</pre>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
}

function EnvTableSection() {
  const vars = [
    ['IODOOM_SCALE', '0', '0 = auto-fit, 1/2/3 = integer scale'],
    ['IODOOM_FPS', '35', 'FPS cap — 35 is Doom\'s native rate'],
    ['IODOOM_VSYNC', '0', '1 to enable VSync'],
    ['IODOOM_FULLSCREEN', '0', '1 to start fullscreen'],
    ['IODOOM_MOUSE', '0', '1 to enable relative mouse look'],
  ];
  return (
    <section className={styles.envSection}>
      <div className="container">
        <h2 className={styles.sectionTitle}>Configure without recompiling</h2>
        <p className={styles.sectionSubtitle}>All options via environment variables.</p>
        <div className={styles.envTable}>
          <div className={styles.envHeader}>
            <span>Variable</span><span>Default</span><span>Description</span>
          </div>
          {vars.map(([v, d, desc]) => (
            <div key={v} className={styles.envRow}>
              <code className={styles.envVar}>{v}</code>
              <code className={styles.envDefault}>{d}</code>
              <span className={styles.envDesc}>{desc}</span>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}

export default function Home(): React.ReactElement {
  const {siteConfig} = useDocusaurusContext();
  return (
    <Layout title={siteConfig.title} description={siteConfig.tagline}>
      <HeroSection />
      <FeaturesSection />
      <QuickStartSection />
      <EnvTableSection />
    </Layout>
  );
}
