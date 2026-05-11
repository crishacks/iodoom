import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';

const config: Config = {
  title: 'iodoom',
  tagline: 'A fast, user-friendly Doom port built for modern platforms.',
  favicon: 'img/favicon.ico',

  future: { v4: true },

  url: 'https://crishacks.github.io',
  baseUrl: '/iodoom/',

  organizationName: 'crishacks',
  projectName: 'iodoom',
  trailingSlash: false,

  onBrokenLinks: 'throw',
  markdown: {
    hooks: {
      onBrokenMarkdownLinks: 'warn',
    },
  },

  i18n: { defaultLocale: 'en', locales: ['en'] },

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.ts',
          editUrl: 'https://github.com/crishacks/iodoom/edit/main/website/',
        },
        blog: false,
        theme: {
          customCss: './src/css/custom.css',
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    image: 'img/iodoom-social.png',
    colorMode: {
      defaultMode: 'dark',
      respectPrefersColorScheme: true,
    },
    announcementBar: {
      id: 'beta',
      content: '⚡ iodoom is in active development — star us on <a href="https://github.com/crishacks/iodoom">GitHub</a>!',
      backgroundColor: '#c0392b',
      textColor: '#fff',
      isCloseable: true,
    },
    navbar: {
      title: 'iodoom',
      logo: {
        alt: 'iodoom logo',
        src: 'img/logo.svg',
        srcDark: 'img/logo-dark.svg',
      },
      style: 'dark',
      hideOnScroll: true,
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'docsSidebar',
          position: 'left',
          label: 'Docs',
        },
        {
          to: '/docs/api',
          label: 'API',
          position: 'left',
        },
        {
          to: '/docs/porting',
          label: 'Porting Guide',
          position: 'left',
        },
        {
          href: 'https://github.com/crishacks/iodoom',
          position: 'right',
          className: 'header-github-link',
          'aria-label': 'GitHub repository',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Documentation',
          items: [
            { label: 'Getting Started', to: '/docs/intro' },
            { label: 'Building', to: '/docs/building' },
            { label: 'Configuration', to: '/docs/configuration' },
            { label: 'Porting Guide', to: '/docs/porting' },
          ],
        },
        {
          title: 'Project',
          items: [
            { label: 'GitHub', href: 'https://github.com/crishacks/iodoom' },
            { label: 'Releases', href: 'https://github.com/crishacks/iodoom/releases' },
            { label: 'Issues', href: 'https://github.com/crishacks/iodoom/issues' },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} iodoom contributors. Based on <a href="https://github.com/ozkl/doomgeneric">doomgeneric</a> by ozkl. Built with Docusaurus.`,
    },
    prism: {
      theme: prismThemes.oneDark,
      darkTheme: prismThemes.oneDark,
      additionalLanguages: ['c', 'bash', 'makefile'],
    },
    algolia: undefined,
  } satisfies Preset.ThemeConfig,
};

export default config;
